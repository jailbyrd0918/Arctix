#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Delegates.h"


// -- delegate -- //

AX_API
SUnicastDelegate
AX_UnicastDelegate_Construct
(VoidPtr listener, FPDelegate callback);

AX_API
Bool
AX_UnicastDelegate_Destruct
(SUnicastDelegate delegate);


// -- delegate handle -- //

AX_API
SUnicastDelegateHandle
AX_UnicastDelegate_Handle_Construct
(void);

AX_API
Bool
AX_UnicastDelegate_Handle_Destruct
(SUnicastDelegateHandle handle);

AX_API
Bool
AX_UnicastDelegate_Handle_BindDelegate
(SUnicastDelegateHandle handle, SUnicastDelegate delegate);

AX_API
Bool
AX_UnicastDelegate_Handle_UnbindDelegate
(SUnicastDelegateHandle handle, SUnicastDelegate delegate);

AX_API
Bool
AX_UnicastDelegate_Handle_Invoke
(VoidPtr sender, SUnicastDelegateHandle handle, VoidPtr data);




