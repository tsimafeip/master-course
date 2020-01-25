if [ $# -eq 0 ]
then
	echo "Please enter a word or a phrase for translation." >&2
	exit 1
fi

if [ $# -gt 2 ]
then
	echo "Maximum amount of input parameters is 2. You can invoke './trans -h' to know more about this service." >&2
	exit 1
fi

if [ "$1" = '--help' ] || [ "$1" = '-h' ]
then
	printf "This is a translation service.\n\n"
	echo "It takes one obligatory parameter: a word or a phrase for the translation."
	printf "Please do not forget to put your phrase into quotes for correct parsing.\n\n"
	echo "Second parameter defines translation languages. It has to be set in 2 formats."
	echo "1) 'source-target' (2 letter abbreaviated, 'ru-en', for instance)."
	echo "2) just target language('en')."
	echo "If only target language is set, source language will be detected automatically. "
	echo "Second parameter is optional. If it is not set, 'ru-en' pair will be used by default."
	exit 0
fi

lang=${2:-ru-en}

response=$(curl -s --request POST 'https://translate.yandex.net/api/v1.5/tr.json/translate' --data "key=trnsl.1.1.20191219T203739Z.e2453b207d482c3a.eb7314ce7e24d0c5e8116ab44202ead954b08ada" --data-urlencode "text=$1" --data "lang=$lang" | python3 -c "import sys, json; print(json.load(sys.stdin)['text'][0])" 2>/dev/null)
if [ $? -ne 0 ]
then
	echo "Error on translation. Please check internet connection and correctness of input parameters."
	echo "You can './trans -h' to know more about this service."
	exit 1
else
	echo $response
fi
