Imports System
Imports System.Drawing
Imports System.Windows.Forms
Imports Microsoft.DirectX
Imports Microsoft.DirectX.Direct3D
Imports Direct3D = Microsoft.DirectX.Direct3D


Public Class GraphicsClass
    Inherits GraphicsSample
    Private target As System.Windows.Forms.PictureBox = nothing
    Private groupBox1 As System.Windows.Forms.GroupBox = nothing
    Private drawingFont As D3DXFont = nothing
    Private destination As Point = New Point(0, 0)

    public Sub New()
        Me.Text = "Zegar.net"
        AddHandler Me.KeyDown, AddressOf Me.OnPrivateKeyDown
        AddHandler Me.KeyUp, AddressOf Me.OnPrivateKeyUp
        drawingFont = New D3DXFont("Arial", System.Drawing.FontStyle.Bold)

        target = New System.Windows.Forms.PictureBox()
        groupBox1 = New System.Windows.Forms.GroupBox()
        target.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        target.Location = New System.Drawing.Point(155, 20)
        target.Name = "pictureBox1"
        target.Size = New System.Drawing.Size(220, 260)
        target.TabIndex = 0
        target.TabStop = False
        target.Anchor = (AnchorStyles.Top Or AnchorStyles.Bottom Or AnchorStyles.Left Or AnchorStyles.Right)
        groupBox1.Anchor = (AnchorStyles.Top Or AnchorStyles.Bottom Or AnchorStyles.Left)
        groupBox1.Location = New System.Drawing.Point(8, 20)
        groupBox1.Name = "groupBox1"
        groupBox1.Size = New System.Drawing.Size(110, 260)
        groupBox1.TabIndex = 1
        groupBox1.TabStop = False
        groupBox1.Text = "Insert Your UI"

        Me.Controls.AddRange(New System.Windows.Forms.Control[] {Me.groupBox1, Me.target})
        Me.RenderTarget = target
    End Sub

    '/ <summary>
    '/ Event Handler for windows messages
    '/ </summary>
    Private Sub OnPrivateKeyDown(sender As Object, e As KeyEventArgs)

        Select Case (e.KeyCode)
            Case Keys.Up:
                destination.X = 1
            Case Keys.Down:
                destination.X = -1
            Case Keys.Left:
                destination.Y = 1
            Case Keys.Right:
                destination.Y = -1
        End Select
    End Sub
    Private Sub OnPrivateKeyUp(sender As Object , e As KeyEventArgs)

        Select Case (e.KeyCode)
                Case Keys.Up:
                Case Keys.Down:
                    destination.X = 0
                Case Keys.Left:
                Case Keys.Right:
                    destination.Y = 0
        End Select
    End Sub

    '/ <summary>
    '/ Called once per frame, the call is the entry point for 3d rendering. This 
    '/ function sets up render states, clears the viewport, and renders the scene.
    '/ </summary>
    Protected Overrides Sub Render()

        'Clear the backbuffer to a Blue color 
        device.Clear(ClearFlags.Target Or ClearFlags.ZBuffer, Color.Blue, 1.0f, 0)
        'Begin the scene
        device.BeginScene()
        drawingFont.DrawText(5, 5, Color.White.ToArgb(), "X: " & destination.X & " Y: " & destination.Y)

        '
        ' TODO: Insert application rendering code here.
        '
        device.EndScene()
    End Sub

    '/ <summary>
    '/ Initialize scene objects.
    '/ </summary>
    Protected Overrides Sub InitializeDeviceObjects()
        drawingFont.InitializeDeviceObjects(device)

        '
        ' TODO: Insert application device initialization code here.
        '
    End Sub

    '/ <summary>
    '/ Called when a device needs to be restored.
    '/ </summary>
    Protected Overrides Sub RestoreDeviceObjects(sender As System.Object, e As System.EventArgs)
        '
        ' TODO: Insert application device restoration code here.
        '
    End Sub

End Class