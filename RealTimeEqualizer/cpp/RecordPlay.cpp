
#include"../hpp/RecordPlay.hpp"


RecordPlay::RecordPlay() {

	recorder = Recorder(BufferSize);
	recorder.start();
	sound = Sound(Wave(BufferSize), SoundLoop::All);
	readPos = 0;
	writePos = 0;
	slow = false;
	//vocoder = std::make_shared<Vocoder>();
}


void RecordPlay::update() {

	soundPos = sound.streamPosSample();

	// ある程度変換後のサンプルがたまったら再生開始
	if (!sound.isPlaying() && writePos > 8820){		
		sound.play();
	}

	if (soundPos < writePos){
		const size_t diff = writePos - soundPos;

		// この数字が小さいと、ずれは小さくなるがノイズが入りやすくなる
		if (diff < 4410 && !slow)
		{
			// 再生が追い付いてきてバッファに余裕がなくなったら再生速度を少し落とす
			sound.setSpeed(0.9);
			slow = true;
		}
		else if (diff >= 4410 && slow)
		{
			sound.setSpeed(1.0);
			slow = false;
		}
	}

	readSize = recorder.getPosSample() - readPos;

	while (readSize){
		const size_t writeSize = Min(sound.lengthSample() - writePos, readSize);


		// Filter処理
		y = Wave(writeSize);
		for (size_t i = 0; i < writeSize; i++) {
			tmp = 0.0;
			for (int j = 0; j < filter.size(); j++) {
				if (0 <= (long)readPos - j)tmp += ((double)recorder.getWave()[readPos + i - j].left / 32768.0)*filter[j];
			}
			y[i] = Waving::DoubleToSample(tmp);
		}
		const WaveSample* src = &y[0];
		sound.fill(writePos, src, writeSize);

		writePos += writeSize;
		if (writePos == BufferSize)
		{
			writePos = 0;
		}

		readPos += writeSize;
		readSize -= writeSize;
	}

	if (recorder.isEnded()){
		readPos = 0;
		recorder.start();
	}

}


void RecordPlay::setFilter(std::vector<double> Filter) {
	filter = Filter;
}