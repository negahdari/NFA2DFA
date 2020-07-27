object Rules: TRules
  Left = 298
  Top = 149
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Set of Internal Symbols (Edges)'
  ClientHeight = 292
  ClientWidth = 196
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 60
    Top = 264
    Width = 75
    Height = 25
    Caption = 'Ok'
    TabOrder = 1
    OnClick = Button1Click
    OnKeyDown = Button1KeyDown
  end
  object StringGrid1: TStringGrid
    Left = 9
    Top = 8
    Width = 177
    Height = 249
    Color = clMenu
    ColCount = 2
    DefaultColWidth = 60
    DefaultRowHeight = 16
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goThumbTracking]
    ScrollBars = ssNone
    TabOrder = 0
    OnKeyDown = StringGrid1KeyDown
    ColWidths = (
      44
      127)
  end
  object GroupBox1: TGroupBox
    Left = 13
    Top = 96
    Width = 171
    Height = 105
    Caption = 'New Rule'
    TabOrder = 2
    Visible = False
    object Label1: TLabel
      Left = 8
      Top = 24
      Width = 28
      Height = 13
      Caption = 'Rule :'
    end
    object Label2: TLabel
      Left = 8
      Top = 48
      Width = 53
      Height = 13
      Caption = 'Description'
    end
    object RuleEdit: TEdit
      Left = 64
      Top = 24
      Width = 97
      Height = 21
      MaxLength = 10
      TabOrder = 0
      OnKeyPress = RuleEditKeyPress
    end
    object DesEdit: TEdit
      Left = 64
      Top = 48
      Width = 97
      Height = 21
      MaxLength = 20
      TabOrder = 1
      OnKeyPress = RuleEditKeyPress
    end
    object AddButton: TButton
      Left = 53
      Top = 72
      Width = 65
      Height = 25
      Caption = '&Add'
      TabOrder = 2
      OnClick = AddButtonClick
      OnKeyPress = RuleEditKeyPress
    end
  end
end
