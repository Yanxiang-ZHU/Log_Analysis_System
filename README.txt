loganalyzer - 日志分析系统

编译：
    make

运行：
    ./loganalyzer
    或者将命令输入文件重定向： ./loganalyzer < test_commands.txt

数据结构设计简述（每个不超300字）：
- 双向链表（LinkedList）：每个节点为 LogNode，包含 timestamp, timeKey, level, module, message，以及 prev/next 指针。链表用于保存所有日志，支持尾插、头插、按行号删除（1-based）。时间比较使用 timeKey（YYYYMMDDHHMMSS 转换为 long long），遍历从 head 到 tail 输出原文。

- 循环队列（CircularQueue）：固定容量 1000 的数组，存储指向链表节点的指针，用于快速获取最近 1000 条日志。队满时覆盖最旧元素。支持按队头到队尾遍历（从旧到新）。删除日志时会扫描并压缩队列以移除对应指针。

- 二叉搜索树（BST）：以模块名为 key，存储 ERROR 计数为 value。插入或查找基于字符串比较构建 BST。中序遍历用于收集所有模块节点，随后按错误数降序（数量相同时模块名字典序）排序输出 STATS。

- 撤销栈（UndoStack）：保存最近最多 5 次 DELETE 操作的系统快照（Snapshot）。快照包含链表的深拷贝、基于拷贝链表重建的 BST，以及指向拷贝链表节点的循环队列数组。UNDO 操作将当前状态替换为快照，恢复链表、队列与 BST。

- KMP：手写 KMP 算法（computeLPS 与匹配函数）用于在消息字段中查找关键词，禁止使用 string::find。

已知问题说明：
- Undo 快照的队列重建在实现中采用从拷贝链表中选取最近元素的简单策略（在极端并发或非常规队列指针混合场景下可能不完美）。本次作业实验中，按题目要求的用例应能正确恢复最近日志顺序。
