#include "Arctix/Core/Delegates/Multicast/Multicast.h"

#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"


Bool
_Ax_MulticastDelegate_Handle_Contains
(SMulticastDelegateHandle handle, SMulticastDelegate delegate)
{
	for (UInt32 i = 0; i < AX_Array_GetLength(handle->delegates); ++i) {
		SMulticastDelegate curr = handle->delegates[i];

		Bool reqCondition =
			(curr->callbacks == delegate->callbacks) &&
			(curr->listener == delegate->listener);

		if (reqCondition)
			return true;
	}

	return false;
}


// -- delegate -- //

AX_API
SMulticastDelegate
AX_MulticastDelegate_Construct
(VoidPtr listener)
{
	SMulticastDelegate delegate = AX_Module_Memory_Allocate(
		sizeof(AX_Delegate_Multicast),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
	);

	if (!delegate) {
		AX_Module_Memory_Deallocate(
			delegate,
			sizeof(AX_Delegate_Multicast),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
		);

		return NULL;
	}

	// init properties
	{
		delegate->callbacks = AX_Array_Construct(FPDelegate);
		delegate->listener = listener;
	}

	return delegate;
}

AX_API
Bool
AX_MulticastDelegate_Destruct
(SMulticastDelegate delegate)
{
	if (!delegate)
		return false;

	delegate->listener = NULL;

	// deallocate callbacks
	AX_Array_Destruct(delegate->callbacks);

	// deallocate the delegate itself
	AX_Module_Memory_Deallocate(
		delegate,
		sizeof(AX_Delegate_Multicast),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
	);

	return true;
}

AX_API
Bool
AX_MulticastDelegate_AddCallback
(SMulticastDelegate delegate, FPDelegate callback)
{
	if (!delegate || !callback)
		return false;

	AX_Array_Push(delegate->callbacks, callback);
	return true;
}

AX_API
Bool
AX_MulticastDelegate_RemoveCallback
(SMulticastDelegate delegate, FPDelegate callback, Bool removeAll)
{
	if (!delegate || !callback)
		return false;

	if (removeAll) {
		for (UInt32 i = 0; i < AX_Array_GetLength(delegate->callbacks); ++i) {
			FPDelegate curr = delegate->callbacks[i];

			if (curr == callback)
				AX_Array_Remove(delegate->callbacks, i);
		}
	}
	else {
		for (UInt32 i = 0; i < AX_Array_GetLength(delegate->callbacks); ++i) {
			FPDelegate curr = delegate->callbacks[i];

			if (curr == callback) {
				AX_Array_Remove(delegate->callbacks, i);
				return true;
			}
		}
	}

	return true;
}


// -- delegate handle -- //

AX_API
SMulticastDelegateHandle
AX_MulticastDelegate_Handle_Construct
(void)
{
	SMulticastDelegateHandle handle = AX_Module_Memory_Allocate(
		sizeof(AX_Delegate_Multicast_Handle),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
	);

	if (!handle) {
		AX_Module_Memory_Deallocate(
			handle,
			sizeof(AX_Delegate_Multicast_Handle),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
		);

		return NULL;
	}

	// init properties
	{
		handle->delegates = AX_Array_Construct(SMulticastDelegate);
		handle->numDelegates = AX_Array_GetLength(handle->delegates);
	}

	return handle;
}

AX_API
Bool
AX_MulticastDelegate_Handle_Destruct
(SMulticastDelegateHandle handle)
{
	if (!handle)
		return false;

	// deallocate and set properties to zero
	{
		AX_Array_Destruct(handle->delegates);
		handle->numDelegates = 0;
	}

	// deallocate the handle itself
	AX_Module_Memory_Deallocate(
		handle,
		sizeof(AX_Delegate_Multicast_Handle),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
	);

	return true;
}

AX_API
Bool
AX_MulticastDelegate_Handle_BindDelegate
(SMulticastDelegateHandle handle, SMulticastDelegate delegate)
{
	if (!handle || !delegate)
		return false;

	if (_Ax_MulticastDelegate_Handle_Contains(handle, delegate))
		return false;

	// bind new delegate - update properties
	{
		AX_Array_Push(handle->delegates, delegate);
		handle->numDelegates = AX_Array_GetLength(handle->delegates);
	}

	return true;
}

AX_API
Bool
AX_MulticastDelegate_Handle_UnbindDelegate
(SMulticastDelegateHandle handle, SMulticastDelegate delegate)
{
	if (!handle || !delegate)
		return false;

	// search and unbind delegate - update properties
	{
		for (UInt32 i = 0; i < AX_Array_GetLength(handle->delegates); ++i) {
			SMulticastDelegate curr = handle->delegates[i];

			Bool reqCondition =
				(curr->callbacks == delegate->callbacks) &&
				(curr->listener == delegate->listener);

			if (reqCondition) {
				AX_Array_Remove(handle->delegates, i);
				handle->numDelegates = AX_Array_GetLength(handle->delegates);
				return true;
			}
		}
	}

	return false;
}

AX_API
Bool
AX_MulticastDelegate_Handle_Invoke
(VoidPtr sender, SMulticastDelegateHandle handle, VoidPtr data)
{
	if (!handle)
		return false;

	// go through the delegates...
	for (UInt32 i = 0; i < AX_Array_GetLength(handle->delegates); ++i) {
		SMulticastDelegate curr = handle->delegates[i];

		// go through the callbacks...
		for (UInt32 j = 0; j < AX_Array_GetLength(curr->callbacks); ++j) {
			FPDelegate callback = curr->callbacks[j];

			// ... invoke them individually
			callback(sender, curr->listener, data);
		}
	}

	return true;
}


