# -*- coding: utf-8 -*-
# www/strona.py

from flask import Flask
from flask import render_template

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/k1')
def klasa1a():
    return render_template('1a.html')

@app.route('/k2')
def klasa1b():
    return render_template('1b.html')

# -*- coding: utf-8 -*-
# www/strona.py

from flask import Flask
from flask import render_template

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/k1')
def klasa1a():
    return render_template('1a.html')

@app.route('/k2')
def klasa1b():
    return render_template('1b.html')

plik = open('F:/Projekty/Ukladanie_planu_lekcji/plan.txt', 'r')
dane = plik.readlines()
dane2 = []

for linia in dane:
    if linia[-1] == '\n':
        dane2.append(linia[:-1])
    else:
        dane2.append(linia)

dane3 = []

for linia in dane2:
    dane3.append(" ".join(linia.split()))

plik.close()
dane = dane3
print(dane)

if __name__ == '__main__':
    app.run(debug=True)