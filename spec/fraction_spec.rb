require 'spec_helper'

describe Trooth::Fraction do
  let(:pos1) { Trooth::Fraction.new('1234/3') }
  let(:neg1) { Trooth::Fraction.new('-1234/3') }
  let(:neg2) { Trooth::Fraction.new('-10/3') }
  let(:pos2) { Trooth::Fraction.new('10/3') }

  context 'initialization' do
    it 'works from a string' do
      test_num = Trooth::Fraction.new('1/2')
      expect(test_num.to_s).to eq('1/2')
    end

    it 'simplifies forms' do
      test_num = Trooth::Fraction.new('5/10')
      expect(test_num.to_s).to eq('1/2')
    end
  end
end
