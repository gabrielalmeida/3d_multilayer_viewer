ffmpeg -y -r 5 -start_number 0 -i '%1' -r 30 -i '%3/audio.mp3' -shortest '%2/teste.mov'

