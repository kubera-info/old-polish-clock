Public Class Form1
    Inherits System.Windows.Forms.Form
    Const kat As Double = Math.PI * 0.6
    Const krok As Integer = 30
    Const wewn As Double = 0.8    

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub
    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    '<System.Diagnostics.DebuggerStepThrough()> 
    Private Sub InitializeComponent()
        '
        'Form1
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.BackColor = System.Drawing.Color.White
        Me.ClientSize = New System.Drawing.Size(504, 300)
        Me.Cursor = System.Windows.Forms.Cursors.SizeAll
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None
        Me.Name = "Form1"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Kreator tekstury"
        Me.TopMost = True
        Me.WindowState = System.Windows.Forms.FormWindowState.Maximized

    End Sub
#End Region
    Protected Overrides Sub OnLoad( _
       ByVal e As EventArgs _
    )
        Beep()
        MsgBox("Proszê zaczekaæ do zakoñczenia rysowania bez modyfikacji stanu okien." + Chr(13) + Chr(10) + "Naciœnij klawisz OK.")
        NarysujTarcze()

        'Zrzut zawartoœci ekranu do schowka
        Windows.Forms.SendKeys.SendWait("%{PRTSC}") 'Prinst Screen
        'Koniec aplikacji
        'Informacja o wykonanym zadaniu
        Beep()
        MsgBox("W schowku znajduje siê gotowy obraz.")

        Me.Close()
        End
    End Sub

    Private Function proporcja(ByVal n As Double)
        Return CInt(n * Math.Min(Me.Height, Me.Width) / 600 + 0.5)
    End Function

    Private Sub NarysujTarcze()
        'Pióra
        Dim pen1 As New Drawing.Pen(System.Drawing.Color.Black, proporcja(3))
        Dim pen2 As New Drawing.Pen(System.Drawing.Color.Black, proporcja(8))
        Dim pens(256) As Pen
        'Wzory liter o doœæ wysokiej rozdzielczoœci
        Dim m_I As New Bitmap("..\..\Zasoby\I.bmp")  'Litera I
        Dim m_V As New Bitmap("..\..\Zasoby\Y.bmp")  'Litera V
        Dim m_X As New Bitmap("..\..\Zasoby\X.bmp")  'Litera X
        Dim m_LewyX As New Bitmap("..\..\Zasoby\XLewySzeryf.bmp")  'Litera sklejona lewym, górnym szeryfem
        Dim m_PrawyX As New Bitmap("..\..\Zasoby\XPrawySzeryf.bmp")  'Litera sklejona prawym, górnym szeryfem

        'Sta³e opisuj¹ce planszê
        Static sr As Double = 300
        Static rmax As Integer = 267
        Static rmin As Integer = 252
        Static rfontmax As Integer = 242
        Static rfontmin As Integer = 173
        Dim krok As Double = Math.PI / 30
        Dim czcionka As Double = Math.PI * 263 / (3000 * 2 * m_X.Width) '40 / 2)

        'Wspó³czynniki, zmienne
        Const rozm = 4000
        Dim kolorMinus%(rozm, rozm) 'Iloœæ ciemnych trafieñ w piksel
        Dim kolorPlus%(rozm, rozm) 'Iloœæ jasnych trafieñ w piksel
        Dim ind, indy As Integer
        Dim m_sin As Double
        Dim m_cos As Double
        'Ka¿da litera w ³añcuchu jest identyfikatorem mapy bitowej
        Dim a_godz() As String = {"PII", "I", "II", "III", "IIII", "V", "VI", "VII", "VIII", "IL", "X", "PI"}
        Dim x%, y%, r%
        Dim gr As Graphics = Me.CreateGraphics

        For x% = 0 To 255 ' Gradacja
            pens(x%) = _
                New Drawing.Pen( _
                    Drawing.Color.FromArgb(255, x%, x%, x%), 1)
        Next

        'Opcja AntiAlias jest zbêdna, 
        'gdy¿ jest realizowana programowo poprzez zliczenie jasnych i ciemnych trafieñ w piksel
        For ind = 0 To 11
            Dim pre As Integer
            Dim szer As Double = 0
            Dim kat As Double
            Dim znak$
            For pre = 1 To Len(a_godz(ind))
                znak$ = Mid(a_godz(ind), pre, 1)
                If znak$ = "V" Then
                    szer = szer + m_V.Width
                ElseIf znak$ = "I" Then
                    szer = szer + m_I.Width
                ElseIf znak$ = "X" Then
                    szer = szer + m_X.Width
                ElseIf znak$ = "L" Then
                    szer = szer + m_LewyX.Width
                ElseIf znak$ = "P" Then
                    szer = szer + m_PrawyX.Width
                Else
                    MsgBox("B³¹d!!!!")
                End If
            Next
            szer = szer * czcionka
            Dim obrazek As Bitmap
            Dim plusik As Double = 0
            For pre = 1 To Len(a_godz(ind))
                'Wybierz mapê bitow¹
                znak$ = Mid(a_godz(ind), pre, 1)
                If znak$ = "V" Then
                    obrazek = m_V
                ElseIf znak$ = "I" Then
                    obrazek = m_I
                ElseIf znak$ = "X" Then
                    obrazek = m_X
                ElseIf znak$ = "L" Then
                    obrazek = m_LewyX
                ElseIf znak$ = "P" Then
                    obrazek = m_PrawyX
                Else
                    MsgBox("B³¹d!!!!")
                End If
                'Narysuj mapê bitow¹ rozci¹gniêt¹ na kole
                Const skroc = 0.125
                Dim px, py As Double
                Dim tmp, m_sin2, m_cos2 As Double
                Dim punkty%, punktx%
                For x% = 0 To obrazek.Width - 1
                    m_sin2 = Math.Sin(ind * Math.PI * 2 / 12)
                    m_cos2 = Math.Cos(ind * Math.PI * 2 / 12)
                    ' For y% = 0 To obrazek.Height - 1
                    For y% = -32 To obrazek.Height + 30
                        px = -m_sin2 * (rfontmax - rfontmin) * (y% / m_V.Height) + sr ' Mapa bitowa "V" ma najwiêksz¹ wysokoœæ
                        py = +m_cos2 * (rfontmax - rfontmin) * (y% / m_V.Height) + sr
                        tmp = (ind * Math.PI * 2 / 12 + (1 - skroc * y% / m_V.Height) * (x% * czcionka - szer / 2 + plusik))
                        r = rfontmax '* (y% / obrazek.Height) '* (-rfontmax + rfontmin) + rfontmax
                        m_sin = Math.Sin(tmp)
                        m_cos = Math.Cos(tmp)
                        punktx% = proporcja(m_sin * r + px)
                        punkty% = proporcja(-m_cos * r + py)
                        If punktx% > 0 And punktx% < 4000 And punkty% > 0 And punkty% < rozm Then
                            If y% < 0 Or y% >= obrazek.Height Then
                                kolorPlus%(punktx%, punkty%) += 1
                            ElseIf obrazek.GetPixel(x%, y%).B = 0 Then 'Czarne?
                                kolorMinus%(punktx%, punkty%) += 1
                            Else
                                kolorPlus%(punktx%, punkty%) += 1
                            End If
                        End If
                    Next
                Next
                plusik = plusik + obrazek.Width * czcionka 'Nastêpna litera dalej
            Next
        Next

        For x% = 5 To Me.Height - 6 'Zamieñ trafienia na barwê
            For y% = 5 To Me.Height - 6
                If kolorPlus%(x%, y%) = 0 And kolorMinus%(x%, y%) = 0 Then
                    r% = 255 ' Brak trafieñ, bia³e t³o
                Else
                    Dim maks As Double = Math.Max(kolorPlus%(x%, y%), kolorMinus%(x%, y%)) * 2
                    r% = CInt(Math.Round((0.5 + CDbl(kolorPlus%(x%, y%)) / maks - CDbl(kolorMinus%(x%, y%)) / maks) * 255))
                    r% = CInt(Math.Min(r%, 255)) ' Zaokr¹glenie poza indeks
                    r% = CInt(Math.Max(r%, 0)) ' Zaokr¹glenie poza indeks
                End If
                gr.DrawRectangle(pens(r%), _
                    x%, _
                    y%, _
                    1, 1)
            Next
        Next

        ' Zewnêtrzne otoczenie liter, rysowane z wyg³adzaniem
        gr.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias
        gr.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.GammaCorrected

        gr.DrawEllipse(pen1, proporcja(sr - rmax), proporcja(sr - rmax), proporcja(rmax * 2), proporcja(rmax * 2))
        gr.DrawEllipse(pen1, proporcja(sr - rmin), proporcja(sr - rmin), proporcja(rmin * 2), proporcja(rmin * 2))
        ' Z³¹czenie elips
        For ind = 0 To 59
            m_sin = Math.Sin(krok * ind)
            m_cos = Math.Cos(krok * ind)
            gr.DrawLine( _
                IIf(ind Mod 5 = 0, pen2, pen1), _
                proporcja(sr - rmax * m_sin), _
                proporcja(sr - rmax * m_cos), _
                proporcja(sr - rmin * m_sin), _
                proporcja(sr - rmin * m_cos))
        Next
    End Sub

End Class
