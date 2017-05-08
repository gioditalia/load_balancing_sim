cmake_minimum_required (VERSION 2.6)
project (load_balancing_sim)
add_executable(lb_sim main.c joblist.c msglist.c utils.c logger.c)
