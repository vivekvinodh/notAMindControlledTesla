## Setup This assumes that python 3.5+ and virtualenv are installed.
Execute `source init_env.sh`. This will create and start a python virtualenv
and install all necessary packages. This assumes that a linux based system is
being used -- if not, you're on your own.

When you're finished, you can exit the virtualenv by executing `deactivate`.

## Data Collection
To collect data: `python collect.py [filename]`. This will output filename in
CSV format. Note: you may have to manually kill the process. There seems to be
a resource leakage (unjoined thread) in the OpenBCI or CSV collector interface.
It's not important for hackathon project, so just ignore. If you commit new
data sets to the repo, please use the `data` directory.

## Model Training
To train and score a model using the data you collected: `python train.py [filename]`.
This will train a model solely for the purposes of seeing if accuracy is high
enough to consider using.

## Inference
To run inference, be sure to plug both the Arduino and BCI bluetooth receiver in.
Run `python inference.py [filename]` where filename is the file with data to
train a model.

Note: the serial ports are hardcoded for now. Intending to fix at a later time.
