using System;

namespace Fox
{
    [AttributeUsage(AttributeTargets.Delegate | AttributeTargets.Event)]
    public class SignalAttribute : Attribute
    {
    }
}
