#### 前言

> 注意：Speex编解码器已经被Opus编解码器淘汰，Speex还是可以继续使用，由于Opus比Speex在各方面都更好，所以鼓励大家切换到Opus，但是Opus只支持编码和解码，不支持噪音抑制、声学回音消除等其他处理功能。 ——引用自【[Speex详解（2019年09月25日更新） - 赤勇玄心行天道 - 博客园.html](https://www.cnblogs.com/gaoyaguo/p/5032920.html)】

我们要用的是降噪和回音消除功能，所以还是用的speex项目，准确地说是使用的speex非编码器部分的speexdsp！



### 1、demo使用

**speexdsp编译：**（源码下载地址：[https://github.com/xiph/speexdsp/tags](https://github.com/xiph/speexdsp/tags)）

```bash
tar xzf speexdsp-SpeexDSP-1.2.0.tar.gz
cd speexdsp-SpeexDSP-1.2.0/
./autogen.sh
./configure --prefix=$PWD/_install
make
make install
```

**demo编译及使用：**

仅提供示例程序，实际使用场景未验证。

```bash
$ make clean && make
$ 
$ ./speexdsp_denoise audio/test_8000_16bit_mono.pcm 8000 out_8000_16bit_mono_denoise.pcm
$ ./speexdsp_agc audio/test_8000_16bit_mono.pcm 8000 100000 out_8000_16bit_mono_agc.pcm
$ ./speexdsp_aec audio/out_8000_16bit_mono_agc.pcm audio/out_8000_16bit_mono_denoise.pcm 8000 out_8000_16bit_mono_aec.pcm
```



### 2、参考文章

 - speexdsp-SpeexDSP-1.2.0/doc/manual.pdf

 - speexdsp-SpeexDSP-1.2.0/libspeexdsp/test*（已放到本demo中的docs/reference\_code/目录）

 - [speex库音频降噪(含代码) - 一度逍遥 - 博客园](https://www.cnblogs.com/riddick/p/6959390.html)

 - [基于Speex的声学回声消除\_叶夜笙歌的博客-CSDN博客\_speex回音消除](https://blog.csdn.net/YJJat1989/article/details/19915381)



### 附录（demo目录架构）

```bash
$ tree
.
├── audio
│   ├── out_8000_16bit_mono_aec.pcm
│   ├── out_8000_16bit_mono_agc.pcm
│   ├── out_8000_16bit_mono_denoise.pcm
│   └── test_8000_16bit_mono.pcm
├── docs
│   ├── manual.pdf
│   ├── reference_code
│   │   ├── testdenoise.c
│   │   ├── testecho.c
│   │   ├── testjitter.c
│   │   ├── testresample2.c
│   │   └── testresample.c
│   ├── speex库音频降噪(含代码) - 一度逍遥 - 博客园.mhtml
│   └── 基于Speex的声学回声消除_叶夜笙歌的博客-CSDN博客_speex回音消除.mhtml
├── include
│   └── speex
│       ├── speexdsp_config_types.h
│       ├── speexdsp_types.h
│       ├── speex_echo.h
│       ├── speex_jitter.h
│       ├── speex_preprocess.h
│       └── speex_resampler.h
├── lib
│   └── libspeexdsp.a
├── main_aec.c
├── main_agc.c
├── main_denoise.c
├── Makefile
└── README.md
```

