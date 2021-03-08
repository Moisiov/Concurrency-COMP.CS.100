# not part of course code
# a helper to convert random location data into a C++ array
#
# https://www.randomlists.com/random-location
raw = '''
8°29′N  76°57′E
Trivandrum, India
52°05′N  4°19′E
The Hague, Netherlands
5°33′N  95°19′E
Banda Aceh, Indonesia
21°25′N  39°49′E
Mecca, Saudi Arabia
23°33′S  46°38′W
São Paulo, Brazil
31°57′S  115°52′E
Perth, Australia
43°15′N  2°55′W
Bilbao, Spain
26°11′N  91°44′E
Guwahati, India
50°27′N  104°36′W
Regina, Canada
32°40′N  115°28′W
Mexicali, Mexico
35°18′S  149°07′E
Canberra, Australia
45°52′S  67°29′W
Comodoro Rivadavia, Argentina
53°31′N  49°25′E
Tolyatti, Russia
4°24′N  114°00′E
Miri, Malaysia
37°34′N  126°59′E
Seoul, South Korea
4°53′N  114°57′E
Bandar Seri Begawan, Brunei
34°41′N  135°48′E
Nara, Japan
33°43′N  73°04′E
Islamabad, Pakistan
44°26′N  26°06′E
Bucharest, Romania
46°36′N  112°02′W
Helena, United States
21°46′S  43°21′W
Juiz de Fora, Brazil
52°38′N  1°18′E
Norwich, United Kingdom
32°57′S  60°40′W
Rosario, Argentina
9°59′N  83°02′W
Limon, Costa Rica
5°47′S  35°12′W
Natal, Brazil
21°02′N  105°51′E
Hanoi, Vietnam
53°32′N  113°30′W
Edmonton, Canada
18°56′S  47°31′E
Antananarivo, Madagascar
30°40′N  104°04′E
Chengdu,China
34°23′N  132°27′E
Hiroshima, Japan
51°42′S  57°52′W
Stanley, United Kingdom
37°58′N  23°43′E
Athens, Greece
24°47′S  65°25′W
Salta, Argentina
65°22′N  27°00′E
Pudasjärvi, Finland
0°33′S  166°55′E
Yaren District, Nauru
'''
import re, hashlib

def dms2dd(degrees, minutes, direction, seconds=0):
    dd = float(degrees) + float(minutes)/60 + float(seconds)/(60*60)
    if direction == 'W' or direction == 'S':
        dd *= -1
    return dd

items = list( filter(lambda x: len(x) > 0, raw.split('\n')) )
COUNTER = 0
while len(items) > 0:
    coord = items.pop(0)
    name = items.pop(0)
    COUNTER += 1
    a,b = coord.split()
    lon = re.split("[°\′]", a)
    lat = re.split("[°\′]", b)
    lon = dms2dd(*lon)
    lat = dms2dd(*lat)
    #print(name, lon, lat)
    hash = hashlib.sha1(bytes(name+coord,encoding='utf8')).hexdigest()
    print( 'std::make_tuple( {}, "{}", "{}", "{}"),'.format(
           '0x'+str(hash)[:15], name, coord, f'https://www.openstreetmap.org/#map=12/{lon:.3f}/{lat:.3f}')
    )
print( COUNTER )
