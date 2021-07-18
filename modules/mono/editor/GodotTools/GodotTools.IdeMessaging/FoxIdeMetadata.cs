namespace FoxTools.IdeMessaging
{
    public readonly struct FoxIdeMetadata
    {
        public int Port { get; }
        public string EditorExecutablePath { get; }

        public const string DefaultFileName = "ide_messaging_meta.txt";

        public FoxIdeMetadata(int port, string editorExecutablePath)
        {
            Port = port;
            EditorExecutablePath = editorExecutablePath;
        }

        public static bool operator ==(FoxIdeMetadata a, FoxIdeMetadata b)
        {
            return a.Port == b.Port && a.EditorExecutablePath == b.EditorExecutablePath;
        }

        public static bool operator !=(FoxIdeMetadata a, FoxIdeMetadata b)
        {
            return !(a == b);
        }

        public override bool Equals(object obj)
        {
            if (obj is FoxIdeMetadata metadata)
                return metadata == this;

            return false;
        }

        public bool Equals(FoxIdeMetadata other)
        {
            return Port == other.Port && EditorExecutablePath == other.EditorExecutablePath;
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return (Port * 397) ^ (EditorExecutablePath != null ? EditorExecutablePath.GetHashCode() : 0);
            }
        }
    }
}
