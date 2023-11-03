#pragma once

#include "Arctix/Defines/Core/DataTypes.h"


typedef	Bool(*AX_Delegate_Function)	(VoidPtr sender, VoidPtr listener, VoidPtr data);
typedef	AX_Delegate_Function		FPDelegate;

typedef
struct AX_Delegate_Unicast
{
	VoidPtr				listener;
	FPDelegate			callback;
}
AX_Delegate_Unicast;

typedef AX_Delegate_Unicast *		SUnicastDelegate;

typedef
struct AX_Delegate_Multicast
{
	VoidPtr				listener;
	FPDelegate *			callbacks;
}
AX_Delegate_Multicast;

typedef AX_Delegate_Multicast *		SMulticastDelegate;

typedef
struct AX_Delegate_Unicast_Handle
{
	SUnicastDelegate *		delegates;
	UInt32				numDelegates;
}
AX_Delegate_Unicast_Handle;

typedef AX_Delegate_Unicast_Handle *	SUnicastDelegateHandle;

typedef
struct AX_Delegate_Multicast_Handle
{
	SMulticastDelegate *		delegates;
	UInt32				numDelegates;
}
AX_Delegate_Multicast_Handle;

typedef AX_Delegate_Multicast_Handle *	SMulticastDelegateHandle;


