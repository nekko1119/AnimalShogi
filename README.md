###対応コンパイラ

* Visual C++ 12.0 以上
* gcc 4.7 以上(予定)

###ビルドするために必要なこと

####使用ライブラリ

* Boost 1.55 以上
* Google Test 1.7.0 以上

####環境変数

* `BOOST_DIR`...boostのディレクトリへのパス
* `GTEST_DIR`...google testのディレクトリへのパス

当然ですが、Visual Studio 2013でビルドする場合はVisual Studio 2013でビルドされたGoogle TestとBoostが必要です。  
gccでビルドする場合も同様。

###用語定義

随時変更追加されていきます。

|英語            |日本語         |
|---------------|---------------|
|animal shogi   |どうぶつしょうぎ     |
|black          |先手           |
|white          |後手           |
|chick          |ひよこ           |
|giraffe        |きりん           |
|elephant       |ぞう            |
|hen            |にわとり         |
|lion           |らいおん         |
|promote        |成り            |
|move           |指し手          |
|piece          |駒             |
|captured piece |持ち駒          |
|record         |棋譜           |
|drop           |打ち           |
|board          |盤            |
|state          |局面          |