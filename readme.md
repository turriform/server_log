### Serverlog summary 

contents:

* hash table with qsort
* multithreaded log parser



Assuming logs are in ./data/logs

```
./bin/prog ./data/logs 4
```
Example output with 4 threads
```
Starting thread 0
Starting thread 1
Starting thread 2
Starting thread 3
-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 34179 
        Load:                   0.51 
        Max Jumps (alpha):      24 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 23477 
        Load:                   0.35 
        Max Jumps (alpha):      13 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 27744 
        Load:                   0.41 
        Max Jumps (alpha):      17 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 32216 
        Load:                   0.48 
        Max Jumps (alpha):      27 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 28593 
        Load:                   0.43 
        Max Jumps (alpha):      15 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 26024 
        Load:                   0.39 
        Max Jumps (alpha):      12 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 29393 
        Load:                   0.44 
        Max Jumps (alpha):      17 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 39442 
        Load:                   0.59 
        Max Jumps (alpha):      43 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   33568 
        Filled:                 11562 
        Load:                   0.34 
        Max Jumps (alpha):      10 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   8389 
        Filled:                 3707 
        Load:                   0.44 
        Max Jumps (alpha):      18 


Finished thread 0

-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 28769 
        Load:                   0.43 
        Max Jumps (alpha):      16 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 23917 
        Load:                   0.36 
        Max Jumps (alpha):      12 


Finished thread 3

-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 30551 
        Load:                   0.46 
        Max Jumps (alpha):      21 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 23399 
        Load:                   0.35 
        Max Jumps (alpha):      12 


Finished thread 2

-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 31524 
        Load:                   0.47 
        Max Jumps (alpha):      24 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   33568 
        Filled:                 20576 
        Load:                   0.61 
        Max Jumps (alpha):      31 


Finished thread 1

-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 29444 
        Load:                   0.44 
        Max Jumps (alpha):      93 


-----------------------------------------
*               Hash table              *
-----------------------------------------
        Size:                   67138 
        Filled:                 27182 
        Load:                   0.40 
        Max Jumps (alpha):      69 


[count: 1633580247      ][value: ./data/logs/access.log.2
[count: 1582234606      ][value: ./data/logs/access.log.5
[count: 1467971589      ][value: ./data/logs/access.log.3
[count: 1322512993      ][value: ./data/logs/access.log.9
[count: 1287357003      ][value: ./data/logs/access.log.6
[count: 1195206775      ][value: ./data/logs/access.log.7
[count: 1191116753      ][value: ./data/logs/access.log.8
[count: 127445571       ][value: ./data/logs/access.log.4
Threads 4
Files 8 
Max files per thread 2

Total bytes 279206359988

Geeting Referals...

Top 10 Refs 

[count: 641720  ][value: Mozilla/5.0 (compatible; SemrushBot/6~bl; +http://www.semrush.com/bot.html)
[count: 198072  ][value: Mozilla/5.0 (compatible; YandexBot/3.0; +http://yandex.com/bots)
[count: 154196  ][value: Mozilla/5.0 (compatible; Linux x86_64; Mail.RU_Bot/2.0; +http://go.mail.ru/help/robots)
[count: 143576  ][value: Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.96 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)
[count: 118784  ][value: Mozilla/5.0 (compatible; AhrefsBot/6.1; +http://ahrefs.com/robot/)
[count: 103556  ][value: Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.4 (KHTML, like Gecko) Chrome/98 Safari/537.4 (StatusCake)
[count: 96042   ][value: Mozilla/5.0 (compatible; bingbot/2.0; +http://www.bing.com/bingbot.htm)
[count: 63696   ][value: Mozilla/5.0 (iPhone; CPU iPhone OS 13_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.0.4 Mobile/15E148 Safari/604.1
[count: 49950   ][value: Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)
[count: 48862   ][value: Mozilla/5.0 (iPhone; CPU iPhone OS 13_4_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.1 Mobile/15E148 Safari/604.1

Geeting Urls...

Top 10 Urls

[count: 66283718898     ][value: -
[count: 824240358       ][value: https://www.google.ru/
[count: 563553564       ][value: https://baneks.site/best/
[count: 436840022       ][value: https://baneks.site/new/
[count: 195916918       ][value: https://www.google.com/
[count: 159127708       ][value: https://baneks.site/-%D0%B0%D0%BB%D0%BB%D0%BE-%D1%8D%D1%82%D0%BE-%D0%BB%D0%B5%D0%B2-%D0%B4%D0%B0-%D1%8D%D1%82%D0%BE-%D0%BB%D0%B5%D0%B2/?p=3
[count: 123974936       ][value: https://baneks.site/
[count: 116023658       ][value: https://baneks.site/best/?p=4
[count: 102986432       ][value: https://baneks.site/new/?p=2
[count: 86184400        ][value: https://baneks.site/best/?p=3
```