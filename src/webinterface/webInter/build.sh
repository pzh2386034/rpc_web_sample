#  php-config文件路径需自行确定，在php安装目录的/bin下
./configure  --with-php-config=/usr/local/Cellar/php/php5.6.38/bin/php-config
make
# 将编译出的扩展so拷贝至httpd的modules目录
make install
cp modules/webInter.so /usr/local/Cellar/httpd/2.4.37/lib/httpd/modules/
