#!/bin/bash
echo d	#删除第一个分区（默认只分一个区）
echo n	#新建分区
echo p	#主分区
echo 1	#主分区号
echo	#回车选择默认起始柱面，默认从柱面1开始
echo	#回车选择默认结束柱面，默认为磁盘最后柱面
#echo +100M #指定分区容量

echo w	#保存分区信息
