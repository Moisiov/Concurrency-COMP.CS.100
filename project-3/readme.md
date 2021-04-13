# Rinnakkaisuus harjoitustyö 3

## Gitin käyttö
Master-haara pidetään siistinä ja siellä aina toimiva/kääntyvä ohjelmaversio.

Development-haarassa ohjelman kehitysversio ja uudet toiminnot luodaan developmentin päältä tehtyihin työhaaroihin.

Uuden haaran luonti lokaalisti komennolla `git checkout -b <haaran nimi>`
Uuden haaran vienti remoteen `git push -u origin <haaran nimi>`

Työhaaran vaihto `git checkout <haaran nimi>`

Pidetään commit-viestit lyhyinä ja ytimekkäinä. Käytetään commiteissa käskymuotoa. E.g."Fix a bug x".

Haarojen yhdistäminen:
Mene haaraan johon toinen haara yhdistetään `git checkout development`
Yhdistä toinen haara tähän haaraan mergellä `git merge <työhaara>`
