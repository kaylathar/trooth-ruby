require 'spec_helper'

describe Trooth::Rational do
  let(:pos1) { Trooth::Rational.new('1234/3') }
  let(:neg1) { Trooth::Rational.new('-1234/3') }
  let(:neg2) { Trooth::Rational.new('-10/3') }
  let(:pos2) { Trooth::Rational.new('10/3') }
  let(:pos3) { Trooth::Rational.new('10/3') }

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

  context 'comparisons' do
    it 'supports equality checks' do
      expect(pos2 == pos3).to eq(true)
      expect(pos3 == pos1).to eq(false)
      expect(pos1 != pos2).to eq(true)
      expect(pos2 != pos1).to eq(true)
    end

    it 'supports inequality checks' do
      expect(pos1 != pos2).to eq(true)
      expect(pos2 != pos1).to eq(true)
      expect(pos3 != pos2).to eq(false)
      expect(pos2 != pos3).to eq(false)
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

  context 'division' do
    it 'can divide two positives' do
      expect((pos1 / pos2).to_s).to eq('37/10')
    end

    it 'can divide two negatives' do
      expect((neg1 / neg2).to_s).to eq('37/10')
    end

    it 'can divide a positive and negative' do
      expect((pos1 / neg2).to_s).to eq('-37/10')
      expect((neg1 / pos2).to_s).to eq('-37/10')
    end
  end

  context 'comparison' do
    it 'supports greater than' do
      expect(pos1 > neg1).to eq(true)
      expect(pos1 > pos2).to eq(true)
      expect(pos2 > pos1).to eq(false)
      expect(neg1 > pos1).to eq(false)
    end

    it 'supports less than' do
      expect(pos1 < neg1).to eq(false)
      expect(pos1 < pos2).to eq(false)
      expect(pos2 < pos1).to eq(true)
      expect(neg1 < pos1).to eq(true)
    end

    it 'supports comparison' do
      expect(pos1 <=> pos2).to eq(1)
      expect(pos2 <=> pos3).to eq(0)
      expect(pos2 <=> pos1).to eq(-1)
      expect(pos1 <=> neg1).to eq(1)
      expect(neg1 <=> pos1).to eq(-1)
    end

    it 'supports greater than or equal to' do
      expect(pos1 >= neg1).to eq(true)
      expect(pos1 >= pos2).to eq(true)
      expect(pos2 >= pos1).to eq(false)
      expect(neg1 >= pos1).to eq(false)
      expect(pos2 >= pos3).to eq(true)
    end

    it 'supports less than or equal to' do
      expect(pos1 <= neg1).to eq(false)
      expect(pos1 <= pos2).to eq(false)
      expect(pos2 <= pos1).to eq(true)
      expect(neg1 <= pos1).to eq(true)
      expect(pos2 <= pos3).to eq(true)
    end
  end
end
