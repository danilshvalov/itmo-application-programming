<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        TextBox1 = New TextBox()
        Label1 = New Label()
        Label2 = New Label()
        TextBox2 = New TextBox()
        Label3 = New Label()
        Label4 = New Label()
        TextBox3 = New TextBox()
        Label5 = New Label()
        Button1 = New Button()
        SuspendLayout()
        ' 
        ' TextBox1
        ' 
        TextBox1.Font = New Font("Segoe UI", 18F)
        TextBox1.Location = New Point(14, 57)
        TextBox1.Name = "TextBox1"
        TextBox1.Size = New Size(312, 39)
        TextBox1.TabIndex = 0
        ' 
        ' Label1
        ' 
        Label1.AutoSize = True
        Label1.Font = New Font("Segoe UI", 18F)
        Label1.Location = New Point(14, 22)
        Label1.Name = "Label1"
        Label1.Size = New Size(298, 32)
        Label1.TabIndex = 1
        Label1.Text = "Левая граница интервала"
        ' 
        ' Label2
        ' 
        Label2.AutoSize = True
        Label2.Font = New Font("Segoe UI", 18F)
        Label2.Location = New Point(14, 112)
        Label2.Name = "Label2"
        Label2.Size = New Size(312, 32)
        Label2.TabIndex = 2
        Label2.Text = "Правая граница интервала"
        ' 
        ' TextBox2
        ' 
        TextBox2.Font = New Font("Segoe UI", 18F)
        TextBox2.Location = New Point(14, 150)
        TextBox2.Name = "TextBox2"
        TextBox2.Size = New Size(312, 39)
        TextBox2.TabIndex = 3
        ' 
        ' Label3
        ' 
        Label3.AutoSize = True
        Label3.Font = New Font("Segoe UI", 18F, FontStyle.Bold)
        Label3.Location = New Point(388, 169)
        Label3.Name = "Label3"
        Label3.Size = New Size(133, 32)
        Label3.TabIndex = 4
        Label3.Text = "Результат:"
        ' 
        ' Label4
        ' 
        Label4.AutoSize = True
        Label4.Font = New Font("Segoe UI", 18F)
        Label4.Location = New Point(527, 169)
        Label4.Name = "Label4"
        Label4.Size = New Size(0, 32)
        Label4.TabIndex = 5
        Label4.TextAlign = ContentAlignment.MiddleCenter
        ' 
        ' TextBox3
        ' 
        TextBox3.Font = New Font("Segoe UI", 18F)
        TextBox3.Location = New Point(14, 246)
        TextBox3.Name = "TextBox3"
        TextBox3.Size = New Size(312, 39)
        TextBox3.TabIndex = 7
        ' 
        ' Label5
        ' 
        Label5.AutoSize = True
        Label5.Font = New Font("Segoe UI", 18F)
        Label5.Location = New Point(14, 208)
        Label5.Name = "Label5"
        Label5.Size = New Size(109, 32)
        Label5.TabIndex = 6
        Label5.Text = "Эпсилон"
        ' 
        ' Button1
        ' 
        Button1.Font = New Font("Segoe UI", 18F)
        Button1.Location = New Point(400, 83)
        Button1.Name = "Button1"
        Button1.Size = New Size(247, 61)
        Button1.TabIndex = 8
        Button1.Text = "Рассчитать"
        Button1.UseVisualStyleBackColor = True
        ' 
        ' Form1
        ' 
        AutoScaleDimensions = New SizeF(7F, 15F)
        AutoScaleMode = AutoScaleMode.Font
        ClientSize = New Size(713, 306)
        Controls.Add(Button1)
        Controls.Add(TextBox3)
        Controls.Add(Label5)
        Controls.Add(Label4)
        Controls.Add(Label3)
        Controls.Add(TextBox2)
        Controls.Add(Label2)
        Controls.Add(Label1)
        Controls.Add(TextBox1)
        Font = New Font("Segoe UI", 9F)
        Name = "Form1"
        Text = "Form1"
        ResumeLayout(False)
        PerformLayout()
    End Sub

    Friend WithEvents TextBox1 As TextBox
    Friend WithEvents Label1 As Label
    Friend WithEvents Label2 As Label
    Friend WithEvents TextBox2 As TextBox
    Friend WithEvents Label3 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents TextBox3 As TextBox
    Friend WithEvents Label5 As Label
    Friend WithEvents Button1 As Button

End Class
