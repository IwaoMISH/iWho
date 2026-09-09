# iWho

Win32 GUI ログ解析・GeoIP 検索ツール

## 概要
iWho は、入力されたログテキストから IP アドレスやドメイン名を自動抽出し、DNS 解析および GeoIP 検索（国・地域・ISP・ASN 等）を行う Win32 GUI アプリケーションです。

## 主な機能
- テキストからの IP アドレス・ドメイン名・ホスト名自動抽出
- Winsock2 / WinINet による DNS 正引き・逆引き処理
- ipwho.is API と連携した GeoIP 情報の取得と表示

## 動作環境
- OS: Windows 10 / 11

## 開発・動作環境
* **Visual C++ 6.0**            (`iWho.dsp`)
* **Visual Studio 2005 - 2008** (`iWho_0x.vcproj`)
* **Visual Studio 2010 - 2019** (`iWho_1x.vcxproj`)
* **Visual Studio 2022 - 2026** (`iWho_2x.vcxproj`)


## 使い方
1. Releases ページから最新の実行ファイルをダウンロードします。
2. アプリケーションを起動し、解析対象のテキストを入力します。

## ライセンス
[ライセンス名を記載]

