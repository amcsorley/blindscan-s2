blindscan-s2
============

A C program to blindscan digital satellite signals.
Taking advantage of the blindscan algorithm in the Linux driver
for the Prof DVB-S2 cards, it will step through a range of transponders,
find the symbol rate,and calculate for an LNB if you give it one.

    Blindscan tool for the Linux DVB S2 API
    
    usage: blindscan-s2
    -b        : run blind scan
    -T        : Tune a specific transponder
    -H        : only scan horizontal polarity
    -V        : only scan vertical polarity
    -N        : no polarity
    -i        : interactive mode
    -m        : monitor signal mode
    -M        : monitor and re-tune each time
    -a number : adapter number (default 0)
    -f number : frontend number (default 0)
    -F number : fec (default auto)
    -s number : starting transponder frequency in MHz (default 950)
    -e number : ending transponder frequency in MHz (default 1450)
    -d number : delivery system 4=DSS 5=DVB-S 6=DVB-S2 (default 0)
    -t number : step value for scan in MHz (default 20)
    -R number : amount of times to try tuning each step (default 1)
    -r number : symbol rate in MSym/s (default 16000)
    -l number : local oscillator frequency of LNB (default 0)
    -2        : enable 22KHz tone (default OFF)
    -c number : use DiSEqC COMMITTED switch position N (1-4)
    -u number : use DiSEqC uncommitted switch position N (1-4)
    -U N.N    : orbital position for USALS
    -G N.N    : site longtude
    -A N.N    : site latitude
    -W number : seconds to wait after usals motor move (default 45)
    -v        : verbose
    
    Example:
    Default scans L-band range in steps of 20 on H and V polarity
    blindscan-s2 -b
    Scan 11700-11900 vertical in steps of 10, and calculate for lof
    blindscan-s2 -b -s 11700 -e 11900 -V -t 10 -l 10750
