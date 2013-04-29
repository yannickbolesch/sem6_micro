Erste Schritte ein Repository anlegen
--------------------------------------

Eine ausführliche Beschreibung kann hier gefunden werden: [Git Grundlagen ein Git Repository anlegen](http://git-scm.com/book/de/Git-Grundlagen-Ein-Git-Repository-anlegen)

### Festlegung der eigenen Identität

Damit Änderungen am Quellcode eindeutig nachvollzogen werden können muss die Identität festgelegt werden:

    $ git config --global user.name "John Doe"
    $ git config --global user.email johndoe@example.com


### Einrichten eines github Zugangs

Zur Veranschaulichung wird ein Zugang auf [github.com](http://github.com) eingerichtet.


### Erstellen eines git Repositories aus bereits vorhandenem Quellcode

Es wird ein einfaches Hello World Programm erstellt und und dies in git importiert

Ein leeres git Repository erzeugen

    $ git init

Hello World unter Versionsverwaltung stellen:

    $ git add hello.c
    $ git add HelloWorld.sln
    $ git add HelloWorld\HelloWorld.vcxproj

### Den aktuellen Status prüfen

    $ git status 

### Die Dateien commiten

    $ git commit -m "initial commit"

### Eine Datei verändern und erneut den Status prüfen

    $ git status

### Eine Datei löschen

    $ rm HelloWorld.c
    $ git status
    
### Wiederherstellen der Datei:

   $ git checkout HelloWorld.c


