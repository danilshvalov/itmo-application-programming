unit Unit2;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Math;

type
  TForm2 = class(TForm)
    Label1: TLabel;
    Label2: TLabel;
    Edit1: TEdit;
    Edit2: TEdit;
    Label3: TLabel;
    Edit3: TEdit;
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
  Form2: TForm2;

implementation

{$R *.dfm}

function F(x: Double): Double;
begin
  F := Math.Power(x, 2);
end;

function Integral(a, b: Double; n: int64): Double;
var
  res: Double;
  h: Double;
  x: Double;
  i: int64;
begin
  h := (b - a) / n;
  res := F(a) + F(b);
  i := 1;
  while i < n do
  begin
    x := a + i * h;
    res := res + 4 * F(x);
    Inc(i);
    x := a + i * h;
    res := res + 2 * F(x);
    Inc(i);
  end;
  res := res * h / 3;
  Integral := res;
end;

function FindSolution(a: Double; b: Double; eps: Double): Double;
var
  current, previous: Double;
  n: int64;
begin
  n := 100;
  current := Integral(a, b, n);
  repeat
    previous := current;
    n := n * 2;
    current := Integral(a, b, n);
  until Abs(current - previous) < eps;
  FindSolution := current;
end;

procedure TForm2.Button1Click(Sender: TObject);
var
  a, b, eps: Double;
begin
  a := StrToFloat(Edit1.Text);
  b := StrToFloat(Edit2.Text);
  eps := StrToFloat(Edit3.Text);

  Label5.Caption := FloatToStrF(FindSolution(a, b, eps), ffFixed, 4, 2);
end;

end.
