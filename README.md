[![N|Solid](https://cldup.com/dTxpPi9lDf.thumb.png)](https://nodesource.com/products/nsolid)
# What's About?


This a set of small tutorials explaining fondammentals of nodejs implementation.


Basically NodeJS is stack of C++ and Javascript source code. The most important part of nodejs is based on ``V8`` which is a JavaScript engine written and maintained by Google and embedded into Chromium, Atom-Shell and other open source projects. It use ``libuv`` as core library to deal with all kind of ``streams`` and ``events``. In addition of other libraries which we list bellow:

  - ``zlib`` for compression/uncompression and HTTP GZip capabilities 
  - ``cares`` for DNS lookup
  - ``openssl`` for crypto and SSL over HTTP
  - ``ICU`` as part of V8 dependencies, for internationalization and unicode support  
  -  ``http_parser`` a small library for HTTP protocol parsing 

You might want to look at code source of those libraries into ``deps`` subdirectory of nodejs code base. I don't recommand so, I think it's preferably to forget about it at this moment and follow up this document.

# For Whos And Why?
I'm writting those tutorials step-by-step for all those who want to hack in nodejs internals and reinvinting the wheel. There are a large community of nodejs developers who understand very much how it works and can dig into JavaScript part very easy, but few of them touch to C++ side.
So those tutorials will help people how want to play under the nodejs hood's. I'm targeting specially people with knowledge/experience in C++ and small Javascript tought.


# So.. How?
As I said before tutorials are split into several parts which introduce concepts one by one. First, ``tuto1`` introduce how we can properly prepare V8 Host environment and initialize the platform to -then- compile&run a simple *hello worl* JS script.

  - ``tuto2`` .....
