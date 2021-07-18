namespace Fox
{
    public interface ISerializationListener
    {
        void OnBeforeSerialize();
        void OnAfterDeserialize();
    }
}
