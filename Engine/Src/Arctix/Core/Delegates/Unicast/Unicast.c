#include "Arctix/Core/Delegates/Unicast/Unicast.h"

#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"


Bool
_Ax_UnicastDelegate_Handle_Contains
(SUnicastDelegateHandle handle, SUnicastDelegate delegate)
{
	for (UInt32 i = 0; i < AX_Array_GetLength(handle->delegates); ++i) {
		SUnicastDelegate curr = handle->delegates[i];

		Bool reqCondition =
			(curr->callback == delegate->callback) &&
			(curr->listener == delegate->listener);

		if (reqCondition)
			return true;
	}

	return false;
}


// -- delegate -- //

AX_API
SUnicastDelegate
AX_UnicastDelegate_Construct
(VoidPtr listener, FPDelegate callback)
{
	SUnicastDelegate delegate = AX_Module_Memory_Allocate(
		sizeof(AX_Delegate_Unicast),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
	);

	if (!delegate) {
		AX_Module_Memory_Deallocate(
			delegate,
			sizeof(AX_Delegate_Unicast),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
		);

		return NULL;
	}

	// init properties
	{
		delegate->listener = listener;
		delegate->callback = callback;
	}

	return delegate;
}

AX_API
Bool
AX_UnicastDelegate_Destruct
(SUnicastDelegate delegate)
{
	if (!delegate)
		return false;

	delegate->listener = NULL;

	// deallocate the delegate itself
	AX_Module_Memory_Deallocate(
		delegate,
		sizeof(AX_Delegate_Unicast),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
	);

	return true;
}


// -- delegate handle -- //

AX_API
SUnicastDelegateHandle
AX_UnicastDelegate_Handle_Construct
(void)
{
	SUnicastDelegateHandle handle = AX_Module_Memory_Allocate(
		sizeof(AX_Delegate_Unicast_Handle),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
	);

	if (!handle) {
		AX_Module_Memory_Deallocate(
			handle,
			sizeof(AX_Delegate_Unicast_Handle),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
		);

		return NULL;
	}

	// init properties
	{
		handle->delegates = AX_Array_Construct(SUnicastDelegate);
		handle->numDelegates = AX_Array_GetLength(handle->delegates);
	}

	return handle;
}

AX_API
Bool
AX_UnicastDelegate_Handle_Destruct
(SUnicastDelegateHandle handle)
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
		sizeof(AX_Delegate_Unicast_Handle),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_DELEGATE)
	);

	return true;
}

AX_API
Bool
AX_UnicastDelegate_Handle_BindDelegate
(SUnicastDelegateHandle handle, SUnicastDelegate delegate)
{
	if (!handle || !delegate)
		return false;

	if (_Ax_UnicastDelegate_Handle_Contains(handle, delegate))
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
AX_UnicastDelegate_Handle_UnbindDelegate
(SUnicastDelegateHandle handle, SUnicastDelegate delegate)
{
	if (!handle || !delegate)
		return false;

	// search and unbind delegate - update properties
	{
		for (UInt32 i = 0; i < AX_Array_GetLength(handle->delegates); ++i) {
			SUnicastDelegate curr = handle->delegates[i];

			Bool reqCondition =
				(curr->callback == delegate->callback) &&
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
AX_UnicastDelegate_Handle_Invoke
(VoidPtr sender, SUnicastDelegateHandle handle, VoidPtr data)
{
	if (!handle)
		return false;

	// go through the delegates...
	for (UInt32 i = 0; i < AX_Array_GetLength(handle->delegates); ++i) {
		SUnicastDelegate curr = handle->delegates[i];
		
		// ... invoke their callback function
		curr->callback(sender, curr->listener, data);
	}

	return true;
}




