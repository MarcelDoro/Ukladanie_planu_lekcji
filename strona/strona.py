# -*- coding: utf-8 -*-
# www/strona.py

from flask import Flask
from flask import render_template

def split_fixed_width(line, width=22, count=5): # rozdzienie nie na podstawie spacji tylko odległości
    return [line[i*width:(i+1)*width].strip() for i in range(count)]

def czytaj_plan(nazwa_pliku):
    with open(nazwa_pliku, encoding='utf-8') as f:
        linie=[linia.rstrip('\n') for linia in f if linia.strip()]

    plany = {}
    i = 0
    while i <len(linie):
        nazwa_klasy = linie[i]
        i += 1
        lekcjie = []
        for _ in range(8):
            if i+1 >= len(linie): break
            nauczyciel = split_fixed_width(linie[i])
            przedmiot = split_fixed_width(linie[i+1])
            lekcjie.append(list(zip(nauczyciel, przedmiot)))
            i += 2
        plany[nazwa_klasy] = lekcjie
    return plany

plany = czytaj_plan('F:/Projekty/Ukladanie_planu_lekcji/plan.txt')

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/k1')
def klasa1a():
    return render_template('1a.html',plan=plany.get('1A', []))

@app.route('/k2')
def klasa1b():
    return render_template('1b.html',plan=plany.get('1B', []))

if __name__ == '__main__':
    app.run(debug=True)