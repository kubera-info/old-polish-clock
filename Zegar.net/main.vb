Imports System
Imports System.Drawing
Imports Container = System.ComponentModel.Container
Imports Microsoft.DirectX
'/ <summary>
'/ The main windows form for the application.
'/ </summary>
Public class MainClass
    Private graphics As GraphicsClass = Nothing

    '/ <summary>
    ' Main entry point of the application.
    '/ </summary>
    Public shared Sub Main()
        Dim m As New MainClass()
    End Sub

    Public Sub New()
        Try
            graphics = New GraphicsClass()
        Catch e As DirectXException
            Return
        End Try
        If graphics.CreateGraphicsSample() Then
            graphics.Run()
        End If
    End Sub
End class