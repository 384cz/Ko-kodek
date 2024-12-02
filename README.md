# Kočkodek
Audio codec (with C port by ChatGPT hehe)
\
Bitrate is 12.8 kbps at 8 khz sample rate, input and output is raw PCM data, 8bit unsigned\
\
Uses DPCM and ternary
\
\
Calling the C binary:\
\
`./enc input.pcm out.cat`\
`./dec out.cat output.pcm`

There is an example encoded file inside this repo, named out.cat, sampled at 8 khz rate
