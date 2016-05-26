require 'spec_helper'

describe Trooth::Rational do
  let(:pos1) { Trooth::Rational.new('1234/3') }
  let(:neg1) { Trooth::Rational.new('-1234/3') }
  let(:neg2) { Trooth::Rational.new('-10/3') }
  let(:pos2) { Trooth::Rational.new('10/3') }

  context 'initialization' do
    it 'works from a string' do
      test_num = Trooth::Rational.new('1/2')
      expect(test_num.to_s).to eq('1/2')
    end

    it 'works from two strings' do
      test_num = Trooth::Rational.new('1', '4')
      expect(test_num.to_s).to eq('1/4')
    end

    it 'works from two fixnums' do
      test_num = Trooth::Rational.new(1, 4)
      expect(test_num.to_s).to eq('1/4')
    end

    it 'works from a mix of fixnums, strings, bigints' do
      num = Trooth::BigInt.new('3')
      test_num = Trooth::Rational.new('1', num)
      expect(test_num.to_s).to eq('1/3')

      test_num = Trooth::Rational.new(num, '2')
      expect(test_num.to_s).to eq('3/2')

      test_num = Trooth::Rational.new(num, 2)
      expect(test_num.to_s).to eq('3/2')

      test_num = Trooth::Rational.new(2, num)
      expect(test_num.to_s).to eq('2/3')

      test_num = Trooth::Rational.new(2, '3')
      expect(test_num.to_s).to eq('2/3')

      test_num = Trooth::Rational.new('3', 2)
      expect(test_num.to_s).to eq('3/2')
    end

    it 'works from two BigInts' do
      num = Trooth::BigInt.new('3')
      den = Trooth::BigInt.new('4')
      test_num = Trooth::Rational.new(num, den)
      expect(test_num.to_s).to eq('3/4')
    end

    it 'simplifies forms' do
      test_num = Trooth::Rational.new('5/10')
      expect(test_num.to_s).to eq('1/2')
    end

    it 'supports negatives' do
      test_num = Trooth::Rational.new('-5/10')
      expect(test_num.to_s).to eq('-1/2')
    end

    it 'simplifies double negatives correctly' do
      test_num = Trooth::Rational.new('-5/-10')
      expect(test_num.to_s).to eq('1/2')
    end

    it 'normalizes negatives consistently' do
      test_num = Trooth::Rational.new('5/-10')
      expect(test_num.to_s).to eq('-1/2')
    end
  end

  context 'addition' do
    it 'works with same denominators' do
      test_num = Trooth::Rational.new('2/3')
      expect((pos2 + test_num).to_s).to eq('4/1')
    end

    it 'works with same denominators and negatives' do
      test_num = Trooth::Rational.new('-2/3')
      expect((pos2 + test_num).to_s).to eq('8/3')
    end

    it 'works with different denominators' do
      test_num = Trooth::Rational.new('1/6')
      expect((pos2 + test_num).to_s).to eq('7/2')
    end
  end
end
