unit Unit1;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Math;

type
  TForm1 = class(TForm)
    Label1: TLabel;
    Edit1: TEdit;
    Edit2: TEdit;
    Label2: TLabel;
    Edit3: TEdit;
    Label3: TLabel;
    Button1: TButton;
    Label4: TLabel;
    Label5: TLabel;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

function F(x: Double): Double;
begin
  // F := x - 13.5;
  F := Math.Power(x, 3) - 12 * Math.Power(x, 2) + 45 * x - 51;
end;

function FindSolution(a: Double; b: Double; eps: Double): Double;
begin
  var
    c: Double;

  repeat
    c := (a + b) / 2;
    if F(a) * F(c) <= 0 then
      b := c
    else
      a := c;
  until b - a < eps;

  FindSolution := (a + b) / 2;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  var
    a, b, eps: Double;
  a := StrToFloat(Edit1.Text);
  b := StrToFloat(Edit2.Text);
  eps := StrToFloat(Edit3.Text);
  Label5.Caption := FloatToStrF(FindSolution(a, b, eps), ffFixed, 4, 2);
end;

end.
