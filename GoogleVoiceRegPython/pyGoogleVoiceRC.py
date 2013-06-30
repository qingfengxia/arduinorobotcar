from tempfile import mkstemp
import subprocess
from os import remove
import json
import urllib2
import sys
 
#
duration=2   # two seconds for one command  
#Frame Rate used by api speech from google
fr=16000
#SOX_PATH 

#on Linux, shell=True is needed for path searching, but too much output

def playvoice(filename):
    print "play prompt voice"
    command = "sox %s  -d"%filename
    print command
    #Using subprocess.PIPE to disable output for Python 2.7
    cmd = subprocess.Popen(command, shell=True)
    cmd.wait()  #does not return properly in python, but fine is command prompt!
    #print output

#using sox subprocess
def recordvoice(filename,seconds):
    print "record speech by SOX auto stop, less than 60 seconds"
    command = "sox -d %s trim 0 00:%02d"%(filename,seconds)
    print command
    #Using subprocess.PIPE to disable output for Python 2.7
    cmd = subprocess.Popen(command,shell=True)
    cmd.wait()  #does not return properly in python, but fine is command prompt!
    #print output
    return filename

#using sox subprocess
def convertvoice(ifile, ofile):
    print ""
    # sox $VOICE.wav $VOICE.flac rate 16k
    command = "sox %s %s rate 16k"%(ifile,ofile)
    #Using subprocess.PIPE to disable output for Python 2.7
    cmd = subprocess.Popen(command,shell=True)
    cmd.wait()


def convert_wav(File, ofile):
    import scikits.audiolab as audiolab
    from scikits.samplerate import resample
    # lastest scikits.audiolab include sound record lib, based on python-alsaaudio
    # if you want make the down sample rate using scipy.signal
    #import scipy.signal

    #using audiolab to read wav file
    Signal, fs = audiolab.wavread(File)[:2]
    #changing the original sample rate to 16000fs fast mode
    Signal = resample(Signal, fr/float(fs), 'sinc_best')
     
    #changing sample rate from audio file using scipy this is a bit slow
    #Signal=scipy.signal.resample(Signal,int(round(len(Getsignal)*fr)/float(fs)),window=None)
     
    # file Format type
    fmt = audiolab.Format('flac', 'pcm16')
    nchannels   = 1
     
    # convert into the file .flac
    ofile =  audiolab.Sndfile(FileNameTmp, 'w', fmt, nchannels, fr)
     
    #writing in the file
    ofile.write_frames(Signal)
    #
    return ofile
    
    
def recognize(voiceFile):
    #Sending to google the file .flac
    url = "https://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=pt-BR"
    flac=open(voiceFile,"rb").read()
    header = {'Content-Type' : 'audio/x-flac; rate=16000'}
    req = urllib2.Request(url, flac, header)
    data = urllib2.urlopen(req)    
    return data
    
    
def parsevoice(File):
    #making a file temp for manipulation
    #cd, FileNameTmp    = mkstemp('TmpSpeechFile.flac')
    FileNameTmp="TmpSpeechFile.flac"
    convertvoice(File, FileNameTmp)
    print "void converted and sending to google" 
    data = recognize(FileNameTmp)
    ret=data.read()  # read from stream
    #print ret
    remove(FileNameTmp)
    j=json.loads(ret)
    print "word reg as:", j["hypotheses"]
    words= j["hypotheses"]
    if len(words)>0:
        return words[0]["utterance"]
    else:
        return ""

def processvoice():
    #play prompt voice
    playvoice("prompt.wav")
    #record command
    wavfile="test.wav"
    voicefile=recordvoice(wavfile, 3)  #3 second
    #
    order=parsevoice(voicefile)
    # complete the command
    print "voice order is recognized as:", order

def loop():
    while(True):  #control-C to stop!
        processvoice()
        
#record prompt.wav as a setup/init
processvoice()

"""
if len(sys.argv)<2 :
    #print 'Usage: %s <audio file.wav>' %sys.argv[0]
    #sys.exit(0)
    #curdir, File=mkstemp('TmpSpeechFile.wav')
    File="test.wav"
    recordvoice(File,duration)
else:
    File=sys.argv[1]
"""
    