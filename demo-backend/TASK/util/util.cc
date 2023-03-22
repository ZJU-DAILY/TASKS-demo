/*

  Copyright (C) 2011 by The Department of Computer Science and
  Technology, Tsinghua University

  Redistribution of this file is permitted under the terms of
  the BSD license.

  Author   : Dong Deng
  Created  : 2014-09-05 11:45:57 
  Modified : 2014-09-09 09:55:51
  Contact  : dd11@mails.tsinghua.edu.cn

*/


#include "util.h" 

void readData(string& filename, vector<string>& recs) {
  string str;
  ifstream input(filename, ios::in);
  while (getline(input, str)) {
    for (auto i = 0; i < str.length(); i++)
      str[i] = tolower(str[i]);
    recs.push_back(str);
  }
}

void splitInfo(std::vector<std::string> &result, std::string str, std::string pattern)
{
    std::string::size_type pos;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
}