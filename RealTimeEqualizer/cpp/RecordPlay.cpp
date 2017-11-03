
#include"../hpp/RecordPlay.hpp"


RecordPlay::RecordPlay() {

	recorder = Recorder(BufferSize);
	recorder.start();
	sound = Sound(Wave(BufferSize), SoundLoop::All);
	readPos = 0;
	writePos = 0;
	slow = false;
	
}


void RecordPlay::update() {

	soundPos = sound.streamPosSample();

	// ������x�ϊ���̃T���v�������܂�����Đ��J�n
	if (!sound.isPlaying() && writePos > 8820){		
		sound.play();
	}

	if (soundPos < writePos){
		const size_t diff = writePos - soundPos;

		// ���̐������������ƁA����͏������Ȃ邪�m�C�Y������₷���Ȃ�
		if (diff < 4410 && !slow)
		{
			// �Đ����ǂ��t���Ă��ăo�b�t�@�ɗ]�T���Ȃ��Ȃ�����Đ����x���������Ƃ�
			sound.setSpeed(0.9);
			slow = true;
		
		}else if (diff >= 4410 && slow){
			sound.setSpeed(1.0);
			slow = false;
		}
	}

	readSize = recorder.getPosSample() - readPos;

	while (readSize){
		
		writeSize = Min(sound.lengthSample() - writePos, readSize);
				
		x = Wave(writeSize);
		y = Wave(writeSize);

		for (size_t i = 0; i < writeSize; i++)x[i] = recorder.getWave()[readPos + i];
	
		// Vocoder����
		if (vocoder->isExecute()) {
			x = vocoder->execute(x);
		}

		// Filter����
		for (size_t i = 0; i < writeSize; i++) {
			tmp = 0.0;
			for (int j = 0; j < filter.size(); j++) {
				if (0 <= i - j)tmp += ((double)x[i].left / 32768.0) * filter[j];
			}
			y[i] = Waving::DoubleToSample(tmp);
		}
		
		const WaveSample* src = &y[0];
		sound.fill(writePos, src, writeSize);


		writePos += writeSize;
		if (writePos == BufferSize){
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


void RecordPlay::setVocoder(std::shared_ptr<Vocoder> voc) {
	vocoder = voc;
}