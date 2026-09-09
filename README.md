# iWho

Win32 GUI GeoIP 検索ツール


## 概要

iWho は、入力されたログテキストから IP アドレスやドメイン名を自動抽出し、DNS 解析および GeoIP 検索（国・地域・ISP・ASN 等）を行う Win32 GUI アプリケーションです。


## 主な機能

* テキストからの IP アドレス・ドメイン名・ホスト名自動抽出
* Winsock2 / WinINet による DNS 正引き・逆引き処理
* ipwho.is API と連携した GeoIP 情報の取得と表示


## 開発・ビルド環境

* **Visual C++ 6.0**            (`iWho.dsp`)
* **Visual Studio 2005 - 2008** (`iWho_0x.vcproj`)
* **Visual Studio 2010 - 2019** (`iWho_1x.vcxproj`)
* **Visual Studio 2022 - 2026** (`iWho_2x.vcxproj`)

対応 OS: Windows 10 / 11（Win32 アプリケーションとして動作）


## ビルド・実行手順

1. お使いの環境に合わせたプロジェクトファイル（`.dsp` / `.vcproj` / `.vcxproj`）を Visual Studio で開きます。
2. ビルド（Release / Win32 または x64）を実行して `iWho.exe` を生成します。
3. 生成された `iWho.exe` を起動し、解析対象のテキストを入力または貼り付けて使用します。


## 免責事項

本コードおよびプログラムの使用により生じたいかなる損害についても著作者は責任を負いません。
引用時は上記 URL を明記してください。

* **作者:** Iwao ( https://mish.work/ )
* **Copyright:** (C) 2026 Iwao. All Rights Reserved.

