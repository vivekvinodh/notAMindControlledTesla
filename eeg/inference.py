import sys

import numpy as np
import serial
from sklearn import decomposition
from sklearn import neighbors

import collect
import train
from util import open_bci

class StreamingInference(object):
  """Handles an input stream -> transform -> outputstream."""
  def __init__(self, model, board, arduino):
    self.model = model
    self.board = board
    self.arduino = arduino

  def start(self):
    board.start(self.handle_sample)
  
  def handle_sample(self, sample):
    y = model.predict(np.array(sample.channel_data).reshape(1, -1))[0]
    print("Prediction: {}".format(y))
    s = 1 if y == 'go' else 0
    ser_val = bytes(chr(s), 'utf8')
    print("Writing '{}' to serial...".format(ser_val))
    arduino_ser.write(ser_val)

if __name__ == '__main__':
  assert len(sys.argv) == 2, "Must provide filename as argument."

  # Read in file.
  data = train.read_clean(sys.argv[1], 'go', 'stop')

  # Split into train/test
  Xtr, Xte, Ytr, Yte = train.prepare_train_test(data)

  # Prepare submodels
  ica = decomposition.FastICA(n_components=6)
  nn = neighbors.KNeighborsClassifier(n_neighbors=3)
  model = train.TransformClassifier(ica, nn)

  # Train model
  model.fit(Xtr, Ytr)
  print("\nModel score: {}\n".format(model.score(Xte, Yte)))

  # Set ports.
  # TODO: remove hardcoded values.
  bci_port = '/dev/tty.usbserial-DQ007SU3'
  arduino_port = '/dev/cu.usbmodem1411' 

  print("Starting connection with OpenBCI on port={}...".format(bci_port))
  board = open_bci.OpenBCIBoard(port=bci_port)

  print("Starting connection with Arduino...")
  arduino_ser = serial.Serial(arduino_port)

<<<<<<< HEAD
  def handle_sample(sample):
    y = model.predict(np.array(sample.channel_data).reshape(1, -1))[0]
    print("Prediction: {}".format(y))
    s = '1' if y == 'go' else '0'
    ser_val = bytes(s, 'utf8')
    print("Writing '{}' to serial...".format(ser_val))
    arduino_ser.write(ser_val)
=======
  streamer = StreamingInference(model, board, arduino)
>>>>>>> collect-infer

  print("Starting stream...")
  streamer.start(handle_sample)
