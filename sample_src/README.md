サンプルソース
==============

pyobj2cで変換/生成した.hが正しく使えるか動作確認するためのサンプル。

以下の環境で動作確認した。

* Windows10 x64 22H2
* gcc 14.2.0 (MSYS2)
* gcc 9.2.0 (MinGW)
* GNU Make 4.4.1 (MSYS2)
* GNU Make 3.81 (MinGW)
* glfw3

以下でコンパイル。

```
make clean
make
```

実行は以下。

```
01_drawobj.exe
or
./01_drawobj.exe
```

カーソルキーの左右で表示するモデルを切り替えられる。

