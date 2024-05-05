Public Class Form1
    Private Function F(x As Double) As Double
        ' Return x - 13.5
        Return Math.Pow(x, 3) - 12 * Math.Pow(x, 2) + 45 * x - 51
    End Function
    Private Function FindSolution(a As Double, b As Double, eps As Double) As Double
        Dim c As Double

        Do
            c = (a + b) / 2
            If F(a) * F(c) <= 0 Then
                b = c
            Else
                a = c
            End If
        Loop Until b - a < eps

        Return (a + b) / 2
    End Function
    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        Dim a As Double
        Dim b As Double
        Dim eps As Double

        a = CDbl(TextBox1.Text)
        b = CDbl(TextBox2.Text)
        eps = CDbl(TextBox3.Text)

        Label4.Text = Format(FindSolution(a, b, eps), "Fixed")
    End Sub
End Class
