// Copyright © 2025 Jean Silva
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
//                            http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.

use regex::Regex;

use crate::{
  branch::Branch,
  node::{matching, Node, UnmatchedPatternError}
};

/// Node of either a statement or an expression.
#[derive(Debug)]
pub(crate) struct OperationNode<'a, N: Node> {
  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32,

  /// Textual representation of this node in the AST.
  text: &'a str,

  /// Branch by which the nodes that are expected to follow this one are defined.
  branch: &'a Branch<'a, N>
}

impl<N> OperationNode<N> {
  /// Instantiates an operation node.
  fn new(text: &str, column: u32, row: u32, branch: &Branch<N>) -> Self {
    OperationNode {
      text,
      column,
      row,
      branch
    }
  }
}

impl<N: Node> Node for OperationNode<N> {
  fn column(&self) -> u32 {
    self.column
  }

  fn row(&self) -> u32 {
    self.row
  }

  fn text(&self) -> Result<&str, UnmatchedPatternError> {
    matching(Regex::new(r"\w*"), self.text)
  }

  fn branch(&self) -> &Branch<N> {
    &self.branch
  }
}

impl<'a, N1: Node> Branch<'_, OperationNode<'_, N1>> {
  /// Denotes that an operation node is expected to follow the current node.
  pub(crate) fn operation<N2: Node, F: Fn(OperationNode<'_, N1>) -> Branch<'_, N2>>(
    mut self,
    current: &impl Node,
    text: &str,
    chain: F
  ) -> &'a Self {
    let next = OperationNode::new(
      text,
      self.column_of(current, text),
      self.row_of(current, text),
      &self
    );
    self.expect(next, chain)
  }
}
