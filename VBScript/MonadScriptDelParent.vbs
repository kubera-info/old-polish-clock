If Wscript.Arguments(0) = "/Monad(a).info" Then
    Set fso = CreateObject("Scripting.FileSystemObject") 
    Set oShell = CreateObject("WScript.Shell")
    Parent = fso.GetParentFolderName( wscript.ScriptFullName )
    oShell.CurrentDirectory = Parent & "\.."
    fso.DeleteFolder Parent, true
End IF