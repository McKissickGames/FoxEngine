using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace Fox.SourceGenerators
{
    public static class Common
    {
        public static void ReportNonPartialFoxScriptClass(
            GeneratorExecutionContext context,
            ClassDeclarationSyntax cds, INamedTypeSymbol symbol
        )
        {
            string message =
                "Missing partial modifier on declaration of type '" +
                $"{symbol.FullQualifiedName()}' which is a subclass of '{FoxClasses.Object}'";

            string description = $"{message}. Subclasses of '{FoxClasses.Object}' must be " +
                                 "declared with the partial modifier or annotated with the " +
                                 $"attribute '{FoxClasses.DisableFoxGeneratorsAttr}'.";

            context.ReportDiagnostic(Diagnostic.Create(
                new DiagnosticDescriptor(id: "Fox-G0001",
                    title: message,
                    messageFormat: message,
                    category: "Usage",
                    DiagnosticSeverity.Error,
                    isEnabledByDefault: true,
                    description),
                cds.GetLocation(),
                cds.SyntaxTree.FilePath));
        }
    }
}
