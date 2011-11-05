object TrafficClientForm: TTrafficClientForm
  Left = 546
  Top = 82
  BorderStyle = bsDialog
  Caption = 'Traffic Client'
  ClientHeight = 452
  ClientWidth = 423
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  PixelsPerInch = 120
  TextHeight = 16
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 423
    Height = 452
    Align = alClient
    TabOrder = 0
    object SettingsPanel: TPanel
      Left = 0
      Top = 0
      Width = 425
      Height = 349
      TabOrder = 0
      object Label5: TLabel
        Left = 16
        Top = 324
        Width = 74
        Height = 16
        Caption = 'Connections'
      end
      object ConnectionsUpDown: TUpDown
        Left = 217
        Top = 320
        Width = 12
        Height = 24
        Associate = NumConnectionsEdit
        Min = 1
        Max = 32648
        Position = 1
        TabOrder = 0
        Wrap = False
      end
      object NumConnectionsEdit: TEdit
        Left = 96
        Top = 320
        Width = 121
        Height = 24
        TabOrder = 1
        Text = '1'
        OnChange = NumConnectionsEditChange
      end
      object GroupBox1: TGroupBox
        Left = 8
        Top = 8
        Width = 409
        Height = 105
        Caption = 'Payload'
        TabOrder = 2
        object Label1: TLabel
          Left = 8
          Top = 24
          Width = 37
          Height = 16
          Caption = 'Name'
        end
        object Label8: TLabel
          Left = 8
          Top = 50
          Width = 21
          Height = 16
          Caption = 'Min'
        end
        object Label9: TLabel
          Left = 8
          Top = 76
          Width = 25
          Height = 16
          Caption = 'Max'
        end
        object Label10: TLabel
          Left = 216
          Top = 24
          Width = 31
          Height = 16
          Caption = 'Time'
        end
        object Label11: TLabel
          Left = 216
          Top = 50
          Width = 58
          Height = 16
          Caption = 'Increment'
        end
        object PayloadComboBox: TComboBox
          Left = 88
          Top = 20
          Width = 121
          Height = 24
          ItemHeight = 16
          TabOrder = 0
          OnChange = PayloadComboBoxChange
          Items.Strings = (
            'fixed'
            'increasing'
            'random')
        end
        object PayloadTimeEdit: TEdit
          Left = 282
          Top = 20
          Width = 121
          Height = 24
          TabOrder = 1
          Text = '1'
        end
        object PayloadIncrementEdit: TEdit
          Left = 282
          Top = 46
          Width = 121
          Height = 24
          TabOrder = 2
          Text = '1'
        end
        object PayloadMinEdit: TEdit
          Left = 88
          Top = 46
          Width = 121
          Height = 24
          TabOrder = 3
          Text = '1'
        end
        object PayloadMaxEdit: TEdit
          Left = 88
          Top = 72
          Width = 121
          Height = 24
          TabOrder = 4
          Text = '1000'
        end
        object PayloadHelpBitBtn: TBitBtn
          Left = 328
          Top = 72
          Width = 75
          Height = 25
          TabOrder = 5
          OnClick = PayloadHelpBitBtnClick
          Kind = bkHelp
        end
      end
      object GroupBox2: TGroupBox
        Left = 8
        Top = 120
        Width = 409
        Height = 105
        Caption = 'Client'
        TabOrder = 3
        object Label2: TLabel
          Left = 8
          Top = 24
          Width = 37
          Height = 16
          Caption = 'Name'
        end
        object Label12: TLabel
          Left = 8
          Top = 50
          Width = 21
          Height = 16
          Caption = 'Min'
        end
        object Label13: TLabel
          Left = 8
          Top = 76
          Width = 25
          Height = 16
          Caption = 'Max'
        end
        object Label14: TLabel
          Left = 216
          Top = 24
          Width = 31
          Height = 16
          Caption = 'Time'
        end
        object Label15: TLabel
          Left = 216
          Top = 50
          Width = 58
          Height = 16
          Caption = 'Increment'
        end
        object ClientComboBox: TComboBox
          Left = 80
          Top = 20
          Width = 121
          Height = 24
          ItemHeight = 16
          TabOrder = 0
          OnChange = ClientComboBoxChange
          Items.Strings = (
            'fixed')
        end
        object ClientMinEdit: TEdit
          Left = 80
          Top = 46
          Width = 121
          Height = 24
          TabOrder = 1
          Text = '1'
        end
        object ClientMaxEdit: TEdit
          Left = 80
          Top = 72
          Width = 121
          Height = 24
          TabOrder = 2
          Text = '1000'
          OnChange = ClientMaxEditChange
        end
        object ClientIncrementEdit: TEdit
          Left = 282
          Top = 46
          Width = 121
          Height = 24
          TabOrder = 3
          Text = '1'
        end
        object ClientTimeEdit: TEdit
          Left = 282
          Top = 20
          Width = 121
          Height = 24
          TabOrder = 4
          Text = '1'
        end
        object ClientHelpBitBtn: TBitBtn
          Left = 328
          Top = 72
          Width = 75
          Height = 25
          TabOrder = 5
          OnClick = ClientHelpBitBtnClick
          Kind = bkHelp
        end
      end
      object GroupBox3: TGroupBox
        Left = 8
        Top = 232
        Width = 409
        Height = 81
        Caption = 'Protocol'
        TabOrder = 4
        object Label3: TLabel
          Left = 8
          Top = 25
          Width = 37
          Height = 16
          Caption = 'Name'
        end
        object Label4: TLabel
          Left = 216
          Top = 25
          Width = 24
          Height = 16
          Caption = 'Port'
        end
        object Label16: TLabel
          Left = 8
          Top = 52
          Width = 51
          Height = 16
          Caption = 'Address'
        end
        object ProtocolComboBox: TComboBox
          Left = 80
          Top = 21
          Width = 121
          Height = 24
          ItemHeight = 16
          TabOrder = 0
          Items.Strings = (
            'tcp'
            'udp')
        end
        object ProtocolPortEdit: TEdit
          Left = 284
          Top = 21
          Width = 109
          Height = 24
          TabOrder = 1
          Text = '11111'
        end
        object PortUpDown: TUpDown
          Left = 390
          Top = 21
          Width = 13
          Height = 24
          Min = 1
          Max = 32600
          Position = 1
          TabOrder = 2
          Wrap = False
          OnClick = PortUpDownClick
        end
        object ProtocolAddressEdit: TEdit
          Left = 80
          Top = 48
          Width = 237
          Height = 24
          TabOrder = 3
          Text = '192.168.2.254'
        end
        object ProtocolHelpBitBtn: TBitBtn
          Left = 328
          Top = 48
          Width = 75
          Height = 25
          TabOrder = 4
          OnClick = ProtocolHelpBitBtnClick
          Kind = bkHelp
        end
      end
      object ConnectionsHelpBitBtn: TBitBtn
        Left = 337
        Top = 317
        Width = 75
        Height = 25
        TabOrder = 5
        OnClick = ConnectionsHelpBitBtnClick
        Kind = bkHelp
      end
    end
    object Panel3: TPanel
      Left = 0
      Top = 352
      Width = 425
      Height = 33
      TabOrder = 1
      object StopBitBtn: TBitBtn
        Left = 117
        Top = 4
        Width = 75
        Height = 25
        Caption = '&Stop'
        Enabled = False
        ModalResult = 3
        TabOrder = 0
        OnClick = StopBitBtnClick
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
      object QuitBitBtn: TBitBtn
        Left = 336
        Top = 4
        Width = 75
        Height = 25
        TabOrder = 1
        OnClick = QuitBitBtnClick
        Kind = bkClose
      end
      object StartBitBtn: TBitBtn
        Left = 8
        Top = 4
        Width = 75
        Height = 25
        Caption = '&Start'
        TabOrder = 2
        OnClick = StartBitBtnClick
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
      object AboutBitBtn: TBitBtn
        Left = 227
        Top = 4
        Width = 75
        Height = 25
        Caption = '&About'
        TabOrder = 3
        OnClick = AboutBitBtnClick
        Kind = bkHelp
      end
    end
    object Panel4: TPanel
      Left = 0
      Top = 388
      Width = 425
      Height = 65
      TabOrder = 2
      object Label6: TLabel
        Left = 8
        Top = 16
        Width = 66
        Height = 16
        Caption = 'Data Read'
      end
      object Label7: TLabel
        Left = 8
        Top = 40
        Width = 73
        Height = 16
        Caption = 'Data Written'
      end
      object DataReadLabel: TLabel
        Left = 104
        Top = 16
        Width = 185
        Height = 16
        AutoSize = False
        Caption = '0'
      end
      object DataWrittenLabel: TLabel
        Left = 104
        Top = 40
        Width = 193
        Height = 16
        AutoSize = False
        Caption = '0'
      end
    end
  end
  object ActivityTimer: TTimer
    Enabled = False
    OnTimer = ActivityTimerTimer
    Left = 180
    Top = 408
  end
end
