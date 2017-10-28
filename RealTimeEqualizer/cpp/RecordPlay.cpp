
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

		// Filter����
		x = sound.getWave();
		y = Wave(x.lengthSample, 0.0);
		double tmp;
		for (size_t i = 0; i < y.lengthSample; i++) {
			tmp = 0.0;
			for (int j = 0; j < (int)filter.size(); j++) {
				if (0 <= i - j)tmp += (((double)x[i - j].left / 32768.0)*filter[j]);
			}
			y[i] = Waving::DoubleToSample(tmp);
		}
		sound.fill(y);
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
		sound.fill(writePos, &recorder.getWave()[readPos], writeSize);

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