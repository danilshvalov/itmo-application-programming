all: puml pdf

.PHONY: puml
puml:
	plantuml puml/* -o ../images/puml

pdf:
	latexmk
	cp build/main.pdf report.pdf
