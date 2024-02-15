# 外付け型ARユニット"Running Vision"
## 概要
- ランニングするときにいちいちスマートウォッチを見て、タイムを確認するのがだるい
- 以下のような感じで普通の眼鏡に外付けできるARユニットを開発し,ランニング中にスマートウォッチを見なくても常時ランニングに関わる情報が見れたら良い
- シースル～ARだといいなぁ～
- 横にタッチパネルがついてて操作できるといいなぁ～
- ChatGPTと合体すること考えてカメラもついてるといいなぁ～
- ChatGPTと合体することを考えてマイクもついてるといいなぁ～

## スケジュール
- 2か月くらいで開発

## イメージ
- 初期イメージ
![syoki](/images/20240211_160508.jpg)
- ユニット決定後
![picture 0](/images/6ef02ae110170b166409d101e9aa2b712501e0f5612cb29792947f01a6fbbcea.jpg)  

## 機能
### ランニング補助システムとしての機能
- 走行ペース表示機能、走行距離表示機能、総走行時間表示機能、走行姿勢警告機能、AIとの競争機能

### カードリッジ機能
- 装着するATOMを変えることでシステムごと機能を切り替えられるようにする
![picture 1](/images/b2b579f020b6f2e7a6d31af36d52364bf78fc45e69e84c8f47195e3da747e59e.jpg)  


## UIデザイン
<iframe style="border: 1px solid rgba(0, 0, 0, 0.1);" width="800" height="450" src="https://www.figma.com/embed?embed_host=share&url=https%3A%2F%2Fwww.figma.com%2Ffile%2F9CvITeSlLWFOAVkPiRbl0N%2FVirtual-Running-UI-Design%3Ftype%3Ddesign%26node-id%3D0%253A1%26mode%3Ddesign%26t%3DuFeco8oc3d86j1wJ-1" allowfullscreen></iframe>

## 材料
- M5 Glass Unit(表示インジケータ)
  - https://docs.m5stack.com/en/unit/Glass2%20Unit
- ATOM Echo(操作＋スピーカー＋マイク)
  - https://www.switch-science.com/products/6347
- GPSユニット
  - https://www.switch-science.com/products/5694
- バッテリーどうしようかなぁ
  - とりあえず普通のポータブルバッテリーでいいか・・・

## ソフトウェア開発
### 設計
#### コンテキスト図

![context](/images/context.drawio.svg)

#### データフローダイアグラム(仮)

![dataflow](/images/dataflow.drawio.svg)


### 参考
- M5Stackドキュメント
  - https://docs.m5stack.com/en/products#/en/unit/gps
- M5Stackユニットサンプルコード
  - https://github.com/m5stack
- GPSユニット製品詳細（M5公式）
  - https://docs.m5stack.com/ja/unit/gps
- M5ATOMにPlatformIOで書き込むまで
  - https://memoteki.net/archives/3108
- GPSUnitについての公式ドキュメント
  - https://github.com/m5stack/m5-docs/blob/master/docs/ja/unit/gps.md




