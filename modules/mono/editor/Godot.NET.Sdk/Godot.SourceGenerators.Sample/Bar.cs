namespace Fox.SourceGenerators.Sample
{
    partial class Bar : Fox.Object
    {
    }

    // Foo in another file
    partial class Foo
    {
    }

    partial class NotSameNameAsFile : Fox.Object
    {
    }
}
