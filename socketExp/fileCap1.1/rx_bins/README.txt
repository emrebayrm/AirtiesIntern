== compiling example codes ==
make

== using tx ==
1) send "maho" string with ethernet type 0x1234
sudo ./tx_raw lo maho 1234

2) send "maho" string with ethernet type 0x4321
sudo ./tx_raw lo maho 4321

== using rx_poll ==
1) rx_poll_file gathers packets from both 0x1234 and 0x4321
sudo ./rx_poll lo

== split/concatenate a file ==
1) check md5 of the original file
md5sum bira_gobegi.mp3
562e76f41d7eddcc60dcf8ad8459da00  bira_gobegi.mp3

2) now split into two parts
split -n 2 bira_gobegi.mp3 new

3) now concatenate them and check md5sum
cat new* > bira_gobegi.mp3
md5sum bira_gobegi.mp3
562e76f41d7eddcc60dcf8ad8459da00  bira_gobegi.mp3

== the challange ==
we will send splitted file from two different ethernet protocols, and you would utilize poll to get data from both side, then concatenate them into single file.

ps: usage of ETH_P_ALL is forbidden :)

== compiling example codes ==
make

== using tx ==
1) send "maho" string with ethernet type 0x1234
sudo ./tx_raw lo maho 1234

2) send "maho" string with ethernet type 0x4321
sudo ./tx_raw lo maho 4321

== using rx_poll ==
1) rx_poll_file gathers packets from both 0x1234 and 0x4321
sudo ./rx_poll lo

== split/concatenate a file ==
1) check md5 of the original file
md5sum bira_gobegi.mp3
562e76f41d7eddcc60dcf8ad8459da00  bira_gobegi.mp3

2) now split into two parts
split -n 2 bira_gobegi.mp3 new

3) now concatenate them and check md5sum
cat new* > bira_gobegi.mp3
md5sum bira_gobegi.mp3
562e76f41d7eddcc60dcf8ad8459da00  bira_gobegi.mp3

== the challange ==
we will send splitted file from two different ethernet protocols, and you would utilize poll to get data from both side, then concatenate them into single file.

ps: usage of ETH_P_ALL is forbidden :)

== steps ==
# split the file
1) split -n 2 bira_gobegi.mp3 new

# send one half with ether type 0x1234
2) sudo ./tx_file lo newaa 1450 1234

# send other half with ether type 0x4321
3) sudo ./tx_file lo newab 1450 4321

# receive halves 
4) cd rx_bins

5) sudo ./rx_file lo 4321

6) sudo ./rx_file lo 1234

# now check the outcome
7) cat new* > bira_gobegi.mp3

8) md5sum bira_gobegi.mp3
562e76f41d7eddcc60dcf8ad8459da00  bira_gobegi.mp3
