# 域名与网络地址
1. 关于保存地址信息的结构体`hostent`, 参数分别为：
    - char* h_name：官方域名
    - char** h_aliases：官方域名外的其他域名
    - int h_addrtye：地址族信息
    - int h_length：ip地址长度（ipv4是4，ipv6是16）
    - char** h_addr_list：最重要的成员，保存域名对应的ip地址，可能一个域名对应好几个ip

2. 域名字符串---->ip地址结构体：gethostbyname()
3. ip地址---->域名信息：gethostbyaddr()
