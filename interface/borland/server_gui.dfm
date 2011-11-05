object ServerGuiForm: TServerGuiForm
  Left = 480
  Top = 199
  BorderStyle = bsDialog
  Caption = 'Traffic Server'
  ClientHeight = 330
  ClientWidth = 443
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 120
  TextHeight = 16
  object SettingsPanel: TPanel
    Left = 0
    Top = 0
    Width = 441
    Height = 217
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 40
      Height = 16
      Caption = 'Server'
    end
    object Label2: TLabel
      Left = 240
      Top = 16
      Width = 36
      Height = 16
      Caption = 'Reply'
    end
    object Label3: TLabel
      Left = 16
      Top = 48
      Width = 99
      Height = 16
      Caption = 'Known Protocols'
    end
    object Label4: TLabel
      Left = 240
      Top = 48
      Width = 157
      Height = 16
      Caption = 'Active Protocols and Ports'
    end
    object Label5: TLabel
      Left = 16
      Top = 183
      Width = 24
      Height = 16
      Caption = 'Port'
    end
    object ServerComboBox: TComboBox
      Left = 56
      Top = 12
      Width = 145
      Height = 24
      Hint = 'Select the served type. Only one type of server known: basic'
      ItemHeight = 16
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      Items.Strings = (
        'basic')
    end
    object ReplyComboBox: TComboBox
      Left = 288
      Top = 12
      Width = 145
      Height = 24
      Hint = 
        'Type of reply provided by server. Echo will echo the payload as ' +
        'received. Ack will just acknowledged with the minimum sized pack' +
        'et.'
      ItemHeight = 16
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      Items.Strings = (
        'ack'
        'echo')
    end
    object KnownProtocolsListBox: TListBox
      Left = 8
      Top = 64
      Width = 121
      Height = 97
      Hint = 
        'The known protocols. Together with the Port should be Added to a' +
        'ctive protocols'
      ItemHeight = 16
      Items.Strings = (
        'udp'
        'tcp')
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
    end
    object ProtocolsListBox: TListBox
      Left = 232
      Top = 64
      Width = 201
      Height = 97
      Hint = 
        'The server will listen for clients on these protocol, port combi' +
        'nations'
      ItemHeight = 16
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
    end
    object AddButton: TButton
      Left = 144
      Top = 64
      Width = 75
      Height = 25
      Hint = 'Add a protocol, port pair to active protocols and ports'
      Caption = '&Add'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnClick = AddButtonClick
    end
    object RemoveButton: TButton
      Left = 144
      Top = 100
      Width = 75
      Height = 25
      Hint = 'Remove protocol, port pair from active protocols and ports'
      Caption = '&Remove'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 5
      OnClick = RemoveButtonClick
    end
    object ClearButton: TButton
      Left = 144
      Top = 136
      Width = 75
      Height = 25
      Hint = 'Clear the whole active protcols and ports list'
      Caption = '&Clear'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 6
      OnClick = ClearButtonClick
    end
    object PortUpDown: TUpDown
      Left = 169
      Top = 179
      Width = 13
      Height = 24
      Min = 1
      Position = 1
      TabOrder = 7
      Wrap = False
      OnClick = PortUpDownClick
    end
    object PortEdit: TEdit
      Left = 48
      Top = 179
      Width = 121
      Height = 24
      Hint = 
        'The port number with the selected protocol is added to the activ' +
        'e protocol list'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 8
      Text = '11111'
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 216
    Width = 441
    Height = 113
    TabOrder = 1
    object Label6: TLabel
      Left = 8
      Top = 40
      Width = 71
      Height = 16
      Caption = 'Bytes Read'
    end
    object Label7: TLabel
      Left = 8
      Top = 64
      Width = 78
      Height = 16
      Caption = 'Bytes Written'
    end
    object BytesReadLabel: TLabel
      Left = 120
      Top = 40
      Width = 193
      Height = 16
      AutoSize = False
      Caption = '0'
    end
    object BytesWrittenLabel: TLabel
      Left = 120
      Top = 64
      Width = 233
      Height = 16
      AutoSize = False
      Caption = '0'
    end
    object AboutBitBtn: TBitBtn
      Left = 243
      Top = 8
      Width = 75
      Height = 25
      Hint = 'About this program'
      Caption = '&About'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = AboutBitBtnClick
      Kind = bkHelp
    end
    object StartButton: TBitBtn
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Hint = 'Start the server listening for client connections'
      Caption = '&Start'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = StartButtonClick
      Glyph.Data = {
        66010000424D6601000000000000760000002800000014000000140000000100
        040000000000F000000000000000000000001000000010000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
        8888888800008888888888888888888800008888888888888888888800008888
        88888888777777780000888888888880000000780000888888888840FBFBF078
        0000888888888480000000880000888888884888888888880000887777748888
        77777778000080000007777000000078000080FFFF044440FBFBF07800008000
        0008788000000088000088888884878888888888000088888888487877777778
        0000888888888480000000780000888888888840FBFBF0780000888888888880
        0000008800008888888888888888888800008888888888888888888800008888
        88888888888888880000}
    end
    object StopButton: TBitBtn
      Left = 125
      Top = 8
      Width = 75
      Height = 25
      Hint = 'Stop the server listening for client connections'
      Caption = '&Stop'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = StopButtonClick
      Glyph.Data = {
        66010000424D6601000000000000760000002800000014000000140000000100
        040000000000F000000000000000000000001000000000000000000000000000
        BF0000BF000000BFBF00BF000000BF00BF00BFBF0000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
        7777777700007777711111111177777700007777199999999917777700007771
        9999999999917777000077199999999999991777000071999999999999999177
        000019999999999999999917000019FFF99F99FFF9F9991700001999F99F99F9
        F9F99917000019FFF99F99F9F9FFF917000019F9999F99F9F9F9F917000019FF
        F9FFF9FFF9FFF917000019999999999999999917000019999999999999999917
        0000719999999999999991770000771999999999999917770000777199999999
        9991777700007777199999999917777700007777711111111177777700007777
        77777777777777770000}
    end
    object QuitButton: TBitBtn
      Left = 360
      Top = 8
      Width = 75
      Height = 25
      Hint = 'Quit, go away. Stop this!'
      Caption = '&Quit'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = QuitButtonClick
      Kind = bkClose
    end
  end
  object ActivityTimer: TTimer
    Enabled = False
    OnTimer = ActivityTimerTimer
    Left = 208
    Top = 272
  end
end
