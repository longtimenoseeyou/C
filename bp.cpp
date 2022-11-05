#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <memory.h>
#include"bp.h"

BpNet::BpNet(int nInput, int nHide, int nOutput)
{
    _a = 0.2;
    _e = 0.01;

    _nInput = nInput;
    _nHide = nHide;
    _nOutput = nOutput;

    Init();

    srand((unsigned)time(NULL));
    //��ʼ�������-������Ȩϵ��
    for (int i = 0; i < _nInput; i++)
        for (int j = 0; j < _nHide; j++)
            _pplfWeight1[i][j] = (double)(rand() % 20000 - 10000) / 100000;// rand() / (double)(RAND_MAX);
        //��ʼ��������-�����Ȩϵ��
    for (int i = 0; i < _nHide; i++)
        for (int j = 0; j < _nOutput; j++)
            _pplfWeight2[i][j] = (double)(rand() % 20000 - 10000) / 100000;// rand() / (double)(RAND_MAX);
        //��ʼ�����������ֵ
    for (int i = 0; i < _nHide; i++)
        _plfb1[i] = (double)(rand() % 20000 - 10000) / 100000;
    //��ʼ����������ֵ
    for (int i = 0; i < _nOutput; i++)
        _plfb2[i] = (double)(rand() % 20000 - 10000) / 100000;
    //ʹ��sigmoid�����
    f = sigmoid;
}

BpNet::~BpNet()
{
    delete[]_plfHideIn;
    delete[]_plfHideOut;
    delete[]_plfOutputIn;
    delete[]_plfOutputOut;
    delete[]_plfb1;
    delete[]_plfb2;

    for (int i = 0; i < _nInput; i++) delete[]_pplfWeight1[i];
    for (int i = 0; i < _nHide; i++) delete[]_pplfWeight2[i];
    delete[]_pplfWeight1;
    delete[]_pplfWeight2;
}

void BpNet::Init()
{
    _pplfWeight1 = new double* [_nInput];
    for (int i = 0; i < _nInput; i++) _pplfWeight1[i] = new double[_nHide];

    _pplfWeight2 = new double* [_nHide];
    for (int i = 0; i < _nHide; i++) _pplfWeight2[i] = new double[_nOutput];

    _plfb1 = new double[_nHide];
    _plfb2 = new double[_nOutput];


    _plfHideIn = new double[_nHide];
    _plfHideOut = new double[_nHide];
    _plfOutputIn = new double[_nOutput];
    _plfOutputOut = new double[_nOutput];
}

void BpNet::Set(double a, double e)
{
    _a = a;
    _e = e;
}

void BpNet::GetBasicInformation(int& nInput, int& nHide, int& nOutput, double& lfA, double& lfE)
{
    nInput = _nInput;
    nHide = _nHide;
    nOutput = _nOutput;
    lfA = _a;
    lfE = _e;
}

double BpNet::sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

void BpNet::Classify(double plfInput[], double plfOutput[])
{
    memset(_plfHideIn, 0, sizeof(double) * _nHide);
    memset(_plfHideOut, 0, sizeof(double) * _nHide);
    memset(_plfOutputIn, 0, sizeof(double) * _nOutput);
    memset(_plfOutputOut, 0, sizeof(double) * _nOutput);

    //����㵽����������򴫲�
    for (int i = 0; i < _nInput; i++)
        for (int j = 0; j < _nHide; j++)
            _plfHideIn[j] += plfInput[i] * _pplfWeight1[i][j];
    for (int j = 0; j < _nHide; j++) _plfHideOut[j] = (*f)(_plfHideIn[j] + _plfb1[j]);

    //�����㵽���������򴫲�
    for (int j = 0; j < _nHide; j++)
        for (int k = 0; k < _nOutput; k++)
            _plfOutputIn[k] += _plfHideOut[j] * _pplfWeight2[j][k];
    for (int k = 0; k < _nOutput; k++) _plfOutputOut[k] = (*f)(_plfOutputIn[k] + _plfb2[k]);

    if (plfOutput != NULL)
        memcpy(plfOutput, _plfOutputOut, sizeof(double) * _nOutput);
}

bool BpNet::Train(int n, double** pplfInput, double** pplfDesire)
{
    double lfE = _e + 1;

    //�����-������Ȩϵ������
    double** pplfDeltaWeight1 = new double* [_nInput];
    for (int i = 0; i < _nInput; i++)
    {
        pplfDeltaWeight1[i] = new double[_nHide];
        memset(pplfDeltaWeight1[i], 0, sizeof(double) * _nHide);
    }

    //������-�����Ȩϵ������
    double** pplfDeltaWeight2 = new double* [_nHide];
    for (int i = 0; i < _nHide; i++)
    {
        pplfDeltaWeight2[i] = new double[_nOutput];
        memset(pplfDeltaWeight2[i], 0, sizeof(double) * _nOutput);
    }

    //���������ֵ����
    double* plfDeltaBias1 = new double[_nHide];
    memset(plfDeltaBias1, 0, sizeof(double) * _nHide);

    //��������ֵ����
    double* plfDeltaBias2 = new double[_nOutput];
    memset(plfDeltaBias2, 0, sizeof(double) * _nOutput);


    long nCount = 0;
    while (lfE > _e)
    {
        lfE = 0;
        //��ÿһ���������д���
        for (int i = 0; i < n; i++)
        {
            double* plfInput = pplfInput[i];        //��������
            double* plfDesire = pplfDesire[i];      //�����������

            //��������ʵ�����plfOutput
            Classify(plfInput, NULL);

            //���������
            double lfEp = 0;
            for (int j = 0; j < _nOutput; j++)
                lfEp += (plfDesire[j] - _plfOutputOut[j]) * (plfDesire[j] - _plfOutputOut[j]) / 2;
            lfE += lfEp;

            //����������-�����Ȩϵ������
            double* plfChange2 = new double[_nOutput];

            for (int j = 0; j < _nOutput; j++)
                plfChange2[j] = _plfOutputOut[j] * (1 - _plfOutputOut[j]) * (plfDesire[j] - _plfOutputOut[j]);
            for (int j = 0; j < _nHide; j++)
                for (int k = 0; k < _nOutput; k++)
                    pplfDeltaWeight2[j][k] = _a * _plfHideOut[j] * plfChange2[k];
            for (int k = 0; k < _nOutput; k++)
                plfDeltaBias2[k] = _a * plfChange2[k];

            //���������-������Ȩϵ������
            double* plfChange1 = new double[_nHide];
            memset(plfChange1, 0, sizeof(double) * _nHide);
            for (int j = 0; j < _nHide; j++)
            {
                for (int k = 0; k < _nOutput; k++)
                    plfChange1[j] += _pplfWeight2[j][k] * plfChange2[k];
                plfChange1[j] *= _plfHideOut[j] * (1 - _plfHideOut[j]);
            }
            for (int j = 0; j < _nInput; j++)
                for (int k = 0; k < _nHide; k++)
                    pplfDeltaWeight1[j][k] = _a * plfInput[j] * plfChange1[k];
            for (int k = 0; k < _nHide; k++)
                plfDeltaBias1[k] = _a * plfChange1[k];

            delete[]plfChange1;
            delete[]plfChange2;

            //����Bp����Ȩֵ
            for (int i = 0; i < _nInput; i++)
                for (int j = 0; j < _nHide; j++)
                    _pplfWeight1[i][j] += pplfDeltaWeight1[i][j];

            for (int i = 0; i < _nHide; i++)
                for (int j = 0; j < _nOutput; j++)
                    _pplfWeight2[i][j] += pplfDeltaWeight2[i][j];

            //����BP�������ֵ
            for (int i = 0; i < _nOutput; i++)
                _plfb2[i] += plfDeltaBias2[i];

            for (int i = 0; i < _nHide; i++)
                _plfb1[i] += plfDeltaBias1[i];
        }
        nCount++;
        if (nCount % 1000 == 0) printf("%lf\n", lfE);
        if (nCount >= 1000000) break;
    }

    for (int i = 0; i < _nInput; i++) delete[]pplfDeltaWeight1[i];
    for (int i = 0; i < _nHide; i++) delete[]pplfDeltaWeight2[i];
    delete[] pplfDeltaWeight1;
    delete[] pplfDeltaWeight2;

    delete[] plfDeltaBias1;
    delete[] plfDeltaBias2;

    printf("������ %ld ��������\n", nCount);

    return true;
}
