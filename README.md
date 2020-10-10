# regex_to_nfa_to_dfa_to_minimizedDfa
aim to convert regular expression to NFA to DFA to minimized DFA

整体思路：
1.正则表达式先转RNP(Reverse Polish Notation);
2.以RNP为基础建立NFA,NFA存储载体为邻接链表(fake邻接链表,用数组模拟的);
3.以NFA为基础建立DFA,DFA存储载体为状态转化表;
4.对DFA进行化简;

一些需要注意的地方：
1.只能识别*|()这些操作符，直接相连则默认为操作符'+',eg.将"ab"认为"a+b";
2.文件输入于input.txt,输出于output.txt,注意TEST_NUM需与input.txt测试用例数相等;
