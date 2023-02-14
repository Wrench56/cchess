if [ -z "$1" ] || [ -z "$2" ]
then
    echo "Usage: ./manualcurl.sh <api_token> <mode>"
fi

if [ $2 = 1 ]
then
    echo "User info:"
    curl https://lichess.org/api/account -H "Authorization: Bearer $1"
    echo
elif [ $2 = 2 ]
then
    echo "Currently playing:"
    curl https://lichess.org/api/account/playing -H "Authorization: Bearer $1"
    echo
elif [ $2 = 3 ] 
then
    if [ -z "$3" ]
    then
        echo "Usage: ... <username_to_challenge>"
    else
        echo "Challenging $3..."
        curl https://lichess.org/api/challenge/$3 -H "Authorization: Bearer $1" "Content-Type: application/x-www-form-urlencoded" -d "" -o /dev/null -s
        echo
    fi
elif [ $2 = 4 ]
then
    if [ -z "$3"]
    then
        echo "Usage: ... <game_id>"
    else
        echo "Streaming..."
        curl -N https://lichess.org/api/board/game/stream/$3 -H "Authorization: Bearer $1" "Content-Type: application/x-www-form-urlencoded"
        echo
    fi
else
    echo "Error: $2 is not a valid mode!"
fi
