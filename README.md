# StringFilter

Class StringFilter是用來找出本文內指定字串的位置的工具。

< Example >

  StringFilter SF("test.txt","filter.txt");

	string add_filter;
	
	SF.StartFilter();

	SF.ListMatchedMap();
  
< 格式說明 >

    test.txt與filter.txt均為utf8格式，其中filter.txt以換行(\n)為區隔
  
< 時間複雜度分析 >

  假設搜尋字的數量為n，本文長度為m。
  直接套用string find方式來處理則時間複雜度為O(nm)，如FilterContent funciton。
  改進構想來自於每個欲搜尋的字串都需要跑過一次本文長度，如果能先將本文利用map建表，那麼每個欲搜尋的字串即可用查表方式來找出可能對應的位置。
  欲搜尋的字串數量越多，則相對加速越多。
  
< 空間複雜度分析 >

  套用string find方式較大的空間只有搜尋字串與本文空間的size。
  建表方式來說則相對應多了超過一倍的本文空間來建表與儲存字元出現位置。
  
