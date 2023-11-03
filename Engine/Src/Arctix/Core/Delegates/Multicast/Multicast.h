#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Delegates.h"


// -- delegate -- //

AX_API
SMulticastDelegate
AX_MulticastDelegate_Construct
(VoidPtr listener);

AX_API
Bool
AX_MulticastDelegate_Destruct
(SMulticastDelegate delegate);

AX_API
Bool
AX_MulticastDelegate_AddCallback
(SMulticastDelegate delegate, FPDelegate callback);

AX_API
Bool
AX_MulticastDelegate_RemoveCallback
(SMulticastDelegate delegate, FPDelegate callback, Bool removeAll);


// -- delegate handle -- //

AX_API
SMulticastDelegateHandle
AX_MulticastDelegate_Handle_Construct
(void);

AX_API
Bool
AX_MulticastDelegate_Handle_Destruct
(SMulticastDelegateHandle handle);

AX_API
Bool
AX_MulticastDelegate_Handle_BindDelegate
(SMulticastDelegateHandle handle, SMulticastDelegate delegate);

AX_API
Bool
AX_MulticastDelegate_Handle_UnbindDelegate
(SMulticastDelegateHandle handle, SMulticastDelegate delegate);

AX_API
Bool
AX_MulticastDelegate_Handle_Invoke
(VoidPtr sender, SMulticastDelegateHandle handle, VoidPtr data);


