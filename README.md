# StringFilter

Class BM_Alg是用來找出本文內指定字串的位置的工具。

< Example >

  BM_Alg BM("test.txt","filter.txt");

  BM_obj.SortFilter(BM_obj.m_filter);
  
  for (auto& f : BM_obj.m_filter) {
	if (f.length() == 0)
		continue;

	BM_obj.BM( f, BM_obj.m_content);
 }
  
< 格式說明 >

    test.txt與filter.txt均為utf8格式，其中filter.txt以換行(\n)為區隔
  
< 時間複雜度分析 >

  Boyer Moore algorithm 最好O(n/m) 最壞 O(n) 在這邊我們還要再乘上關鍵字的數量
  
