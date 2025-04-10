require 'spec_helper'

describe Trooth::BigInt do
  let(:pos1) { Trooth::BigInt.new('1234') }
  let(:pos3) { Trooth::BigInt.new('1234') }
  let(:neg1) { Trooth::BigInt.new('-1234') }
  let(:neg2) { Trooth::BigInt.new('-10') }
  let(:pos2) { Trooth::BigInt.new('10') }

  context 'initialization' do
    it 'works from a string' do
      test_num = Trooth::BigInt.new('12345')
      expect(test_num.to_s).to eq('12345')
    end

    it 'works from FixNum' do
      test_num = Trooth::BigInt.new(12_345)
      expect(test_num.to_s).to eq('12345')
    end

    it 'works from a Float' do
      test_num = Trooth::BigInt.new(12_345.10)
      expect(test_num.to_s).to eq('12345')
    end

    it 'fails for unknown types' do
      # Test class that is not numeric type
      tmp_class = Class.new do
        def initialize
        end
      end

      expect { Trooth::BigInt.new(tmp_class.new) }.to raise_error
    end

    it 'supports += operator' do
      test_num = Trooth::BigInt.new('1234')
      test_num += Trooth::BigInt.new('1')
      expect(test_num.to_s).to eq('1235')
    end

    it 'supports -= operator' do
      test_num = Trooth::BigInt.new('1234')
      test_num -= Trooth::BigInt.new('1')
      expect(test_num.to_s).to eq('1233')
    end

    it 'supports *= operator' do
      test_num = Trooth::BigInt.new('123')
      test_num *= Trooth::BigInt.new('2')
      expect(test_num.to_s).to eq('246')
    end

    it 'supports /= operator' do
      test_num = Trooth::BigInt.new('222')
      test_num /= Trooth::BigInt.new('2')
      expect(test_num.to_s).to eq('111')
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

    it 'supports equal to' do
      expect(pos1 == pos3).to eq(true)
      expect(pos2 == pos1).to eq(false)
    end

    it 'supports not equal to' do
      expect(pos1 != pos3).to eq(false)
      expect(pos2 != pos1).to eq(true)
    end

    it 'supports comparison' do
      expect(pos1 <=> pos2).to eq(1)
      expect(pos1 <=> pos3).to eq(0)
      expect(pos2 <=> pos1).to eq(-1)
      expect(pos1 <=> neg1).to eq(1)
      expect(neg1 <=> pos1).to eq(-1)
    end

    it 'supports greater than or equal to' do
      expect(pos1 >= neg1).to eq(true)
      expect(pos1 >= pos2).to eq(true)
      expect(pos2 >= pos1).to eq(false)
      expect(neg1 >= pos1).to eq(false)
      expect(pos1 >= pos3).to eq(true)
    end

    it 'supports less than or equal to' do
      expect(pos1 <= neg1).to eq(false)
      expect(pos1 <= pos2).to eq(false)
      expect(pos2 <= pos1).to eq(true)
      expect(neg1 <= pos1).to eq(true)
      expect(pos1 <= pos3).to eq(true)
    end
  end

  context 'conversion' do
    it 'supports conversion to string' do
      test_num = Trooth::BigInt.new(12)
      expect(test_num.to_s).to eq('12')
    end

    it 'supports conversion to integers' do
      test_num = Trooth::BigInt.new(12_345)
      expect(test_num.to_i).to eq(12_345)
    end
  end

  context 'exponentiation' do
    it 'supports exponentiation operator' do
      test_num = Trooth::BigInt.new(2)
      test_expo = Trooth::BigInt.new(4)
      expect((test_num**test_expo).to_i).to eq(16)
    end
  end

  context 'addition' do
    it 'can add two positives' do
      expect((pos1 + pos2).to_s).to eq('1244')
      expect((pos2 + pos1).to_s).to eq('1244')
    end
    it 'can add two negatives' do
      expect((neg1 + neg2).to_s).to eq('-1244')
      expect((neg2 + neg1).to_s).to eq('-1244')
    end
    it 'can add a positive and negative' do
      expect((pos2 + neg2).to_s).to eq('0')
      expect((neg1 + pos1).to_s).to eq('0')
      expect((neg1 + pos2).to_s).to eq('-1224')
      expect((neg2 + pos1).to_s).to eq('1224')
    end
  end

  context 'multiply' do
    it 'can multiply two positives' do
      expect((pos1 * pos2).to_s).to eq('12340')
      expect((pos2 * pos1).to_s).to eq('12340')
    end

    it 'can multiply two negatives' do
      expect((neg1 * neg2).to_s).to eq('12340')
      expect((neg2 * neg1).to_s).to eq('12340')
    end

    it 'can multiply a positive and negative' do
      expect((pos2 * neg2).to_s).to eq('-100')
      expect((neg1 * pos1).to_s).to eq('-1522756')
      expect((neg1 * pos2).to_s).to eq('-12340')
      expect((neg2 * pos1).to_s).to eq('-12340')
    end
  end

  context 'subtract' do
    it 'can subtract two positives' do
      expect((pos1 - pos2).to_s).to eq('1224')
      expect((pos2 - pos1).to_s).to eq('-1224')
    end

    it 'can subtract two negatives' do
      expect((neg1 - neg2).to_s).to eq('-1224')
      expect((neg2 - neg1).to_s).to eq('1224')
    end

    it 'can subtract a positive and negative' do
      expect((pos2 - neg2).to_s).to eq('20')
      expect((neg1 - pos1).to_s).to eq('-2468')
      expect((neg1 - pos2).to_s).to eq('-1244')
      expect((neg2 - pos1).to_s).to eq('-1244')
    end
  end

  context 'divide' do
    it 'can divide two positives' do
      expect((pos1 / pos2).to_s).to eq('123')
      expect((pos2 / pos1).to_s).to eq('0')
    end

    it 'can divide two negatives' do
      expect((neg1 / neg2).to_s).to eq('123')
      expect((neg2 / neg1).to_s).to eq('0')
    end

    it 'can divide a positive and negative' do
      expect((pos2 / neg2).to_s).to eq('-1')
      expect((neg1 / pos1).to_s).to eq('-1')
      expect((neg1 / pos2).to_s).to eq('-123')
      expect((neg2 / pos1).to_s).to eq('0')
    end
  end

  context 'modulo' do
    it 'can modulo two positives' do
      expect((pos1 % pos2).to_s).to eq('4')
      expect((pos2 % pos1).to_s).to eq('10')
    end

    it 'can modulo two negatives' do
      expect((neg1 % neg2).to_s).to eq('4')
      expect((neg2 % neg1).to_s).to eq('10')
    end

    it 'can modulo a positive and negative' do
      expect((pos2 % neg2).to_s).to eq('0')
      expect((neg1 % pos1).to_s).to eq('0')
      expect((neg1 % pos2).to_s).to eq('4')
      expect((neg2 % pos1).to_s).to eq('10')
    end
  end
end
