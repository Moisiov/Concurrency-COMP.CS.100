# Rinnakkaisuus harjoitustyö 3

## Gitin käyttö
Master-haara pidetään siistinä ja siellä aina toimiva/kääntyvä ohjelmaversio.

Development-haarassa ohjelman kehitysversio ja uudet toiminnot luodaan developmentin päältä tehtyihin työhaaroihin.

Uuden haaran luonti lokaalisti komennolla `git checkout -b <branch>`  
Uuden haaran vienti remoteen `git push -u <remote> <branch>`

Vastaavasti haaran poisto lokaalisti tapahtuu komennolla `git branch -d <branch>`  
Ja haaran poisto remotesta `git push -d <remote> <branch>`

Työhaaran vaihto `git checkout <branch>`

Pidetään commit-viestit lyhyinä ja ytimekkäinä. Käytetään commiteissa käskymuotoa. E.g."Fix a bug x".

Haarojen yhdistäminen:  
Mene haaraan johon toinen haara yhdistetään `git checkout <main branch>`  
Yhdistä toinen haara tähän haaraan mergellä `git merge <feature branch>`

Jos fast-forward ei onnistu (tulee merge konflikteja), korjaa konfliktit manuaalisesti.  
Korjausten jälkeen committaa muutokset esim viestillä "Fix merge conflicts" ja suorita merge.
